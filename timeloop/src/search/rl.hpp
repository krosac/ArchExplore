/* Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <iterator>
#include <unordered_set>
#include <fstream>
#include <iostream>

#include "mapping/mapping.hpp"
#include "mapspaces/mapspace-base.hpp"
#include "util/misc.hpp"
#include "search/search.hpp"
#include "communicator.h"
using namespace gym_client;
namespace search
{

class RLSearch : public SearchAlgorithm
{
 private:
  enum class State
  {
    Ready,
    WaitingForStatus,
    Terminated
  };
  
 private:
  // Config.
  mapspace::MapSpace* mapspace_;
  unsigned id_;

  // Live state.
  State state_;
  std::array<uint128_t, unsigned(mapspace::Dimension::Num)> iterator_;
  uint128_t valid_mappings_;
  std::uint64_t eval_fail_count_;

  double best_cost_;
  std::ofstream best_cost_file_;

 public:
  Communicator* comm_;
  std::vector<std::vector<float>> obs_;
  int envs;
  int samples;
  std::unordered_map<unsigned, unsigned> bound;
  double last_cost_;
  double best;
  
  RLSearch(config::CompoundConfigNode config, mapspace::MapSpace* mapspace, unsigned id) :
      SearchAlgorithm(),
      mapspace_(mapspace),
      id_(id),
      state_(State::Ready),
      valid_mappings_(0),
      eval_fail_count_(0),
      best_cost_(0),
      envs(0),
      samples(0),
      last_cost_(0),
      best(0)
  {
    comm_ = new Communicator("tcp://*:10201");
    comm_->ConnectClient();
    auto res = comm_->get_response<MakeParam>("make");
    std::cout << "make::env_name:" << res->env_name << " "
              << "num_envs:" << res->num_envs << "\n";
    envs = res->num_envs;
    comm_->SendMakeResponse("OK");
    comm_->SendInfoResponse(
      "Discrete", {unsigned(mapspace::Dimension::Num) * 2}, 
      "Discrete", {unsigned(mapspace::Dimension::Num)}
    );
    auto res_reset = comm_->get_response<ResetParam>("reset");
    std::cout << "reset::x:" << res_reset->x << "\n";
    for (int e = 0; e < res->num_envs; e++) {
      std::vector<float> tmp;
      for (int i = 0; i < 4; i++) {
        //tmp.push_back(rand() / double(RAND_MAX));
        tmp.push_back(0.5);
      }
      obs_.push_back(tmp);
    }
    comm_->SendMlpResetResponse(obs_);
    comm_->RecvStepReq();

    (void) config;
    
    /*
    for (unsigned i = 0; i < unsigned(mapspace::Dimension::Num); i++)
    {
      iterator_[i] = 0;
    }
    */


    // Special case: if the index factorization space has size 0
    // (can happen with residual mapspaces) then we init in terminated
    // state.
    if (mapspace_->Size(mapspace::Dimension::IndexFactorization) == 0)
    {
      state_ = State::Terminated;
    }
    else
    {
      std::cout << mapspace_->Size(mapspace::Dimension::LoopPermutation) << "\n";
      // Prune the mapspace for the first time.
      mapspace_->InitPruned(0);
      std::cout << mapspace_->Size(mapspace::Dimension::LoopPermutation) << "\n";
    }
    for (auto dim : dim_order_) {
      bound[unsigned(dim)] = (unsigned)mapspace_->Size(dim);
      iterator_[unsigned(dim)] = uint128_t(obs_[0][unsigned(dim)] * (float)bound[unsigned(dim)]);
    }
    std::cerr << "initial MAPPING ID: IF(" << iterator_[unsigned(mapspace::Dimension::IndexFactorization)]
              << ") P(" << iterator_[unsigned(mapspace::Dimension::LoopPermutation)]
              << ") B(" << iterator_[unsigned(mapspace::Dimension::DatatypeBypass)]
              << ") S(" << iterator_[unsigned(mapspace::Dimension::Spatial)]
              << ")" << std::endl;
              
#ifdef DUMP_COSTS
    // Dump best cost for each index factorization.
    best_cost_file_.open("/tmp/timeloop-if-cost.txt");
#endif
  }

  ~RLSearch()
  {
    best_cost_file_.close();
  }

  // Order:
  //   DatatypeBypass <- Spatial <- LoopPermutation <- IndexFactorization.
  std::vector<mapspace::Dimension> dim_order_ =
  {
    mapspace::Dimension::DatatypeBypass,
    mapspace::Dimension::Spatial,
    mapspace::Dimension::LoopPermutation,
    mapspace::Dimension::IndexFactorization
  };
  
  bool IncrementRecursive_(int position = 0)
  {
    auto dim = dim_order_[position];
    if (iterator_[unsigned(dim)] + 1 < mapspace_->Size(dim))
    {
      // Move to next integer in this mapspace dimension.
      iterator_[unsigned(dim)]++;
      if (dim == mapspace::Dimension::IndexFactorization)
      {
        // We just changed the index factorization. Prune the sub-mapspace
        // for this specific factorization index.
        mapspace_->InitPruned(iterator_[unsigned(dim)]);

#ifdef DUMP_COSTS
        // Dump the best cost observed for this index factorization.
        // Note: best_cost_ == 0 implies this was a bad index factorization
        // that failed mapping. We can choose to not report these, or
        // grep them out in post-processing.
        best_cost_file_ << best_cost_ << std::endl;
#endif
        
        // Reset the best cost.
        best_cost_ = 0;
      }
      return true;
    }
    // Carry over to next higher-order mapspace dimension.
    else if (position + 1 < int(mapspace::Dimension::Num))
    {
      iterator_[unsigned(dim)] = 0;
      return IncrementRecursive_(position + 1);
    }
    else
    {
      // Overflow! We are done.
      return false;
    }
  }

  bool Next(mapspace::ID& mapping_id)
  {
    if (state_ == State::Terminated)
    {
      return false;
    }

    assert(state_ == State::Ready);       
    
    mapping_id = mapspace::ID(mapspace_->AllSizes());
    /*for (unsigned i = 0; i < unsigned(mapspace::Dimension::Num); i++)
    {
      mapping_id.Set(i, iterator_[i]);
    }
    */
    for (unsigned i = 0; i < unsigned(mapspace::Dimension::Num); i++)
    {
      //std::cout << "Set " << iterator_[i] << " base " << mapping_id.Base()[i] << "\n";
      mapping_id.Set(i, iterator_[i]);
    }
    state_ = State::WaitingForStatus;
    /*
    std::cerr << "MAPPING ID: IF(" << iterator_[unsigned(mapspace::Dimension::IndexFactorization)]
              << ") P(" << iterator_[unsigned(mapspace::Dimension::LoopPermutation)]
              << ") B(" << iterator_[unsigned(mapspace::Dimension::DatatypeBypass)]
              << ") S(" << iterator_[unsigned(mapspace::Dimension::Spatial)]
              << ")" << std::endl;
    if (index == 50) exit(1);
    */
    
    return true;
  }

  unsigned StepSize(mapspace::Dimension dim) {
    if (rand() / double(RAND_MAX) < 0.01) {
      return unsigned(ceil((float)bound[unsigned(dim)] * 0.01));
    } else {
      return 1;
    }
  }
  
  
  void Report(Status status, double cost = 0)
  {
    assert(state_ == State::WaitingForStatus);
    std::cout << "[" << samples << "]\t";
    std::cerr << "MAPPING ID: IF(" << iterator_[unsigned(mapspace::Dimension::IndexFactorization)]
              << ") P(" << iterator_[unsigned(mapspace::Dimension::LoopPermutation)]
              << ") B(" << iterator_[unsigned(mapspace::Dimension::DatatypeBypass)]
              << ") S(" << iterator_[unsigned(mapspace::Dimension::Spatial)]
              << ")" << std::endl;
    LogStats(status, cost);
    // observation
    obs_.clear();
    for (int e = 0; e < envs; e++) {
      std::vector<float> tmp (unsigned(mapspace::Dimension::Num), 0);
      for (auto dim : dim_order_) {
        tmp[unsigned(dim)] = (float)iterator_[unsigned(dim)]/ (float)bound[unsigned(dim)];
        //std::cout << (float)iterator_[unsigned(dim)] << " / " << (float)bound[unsigned(dim)] << "\n";
      }
      obs_.push_back(tmp);
    }
    // reward
    float custom_cost = -(float)cost/100000000;
    if (cost == 0) {
      custom_cost = -100;
    }
    std::vector<std::vector<float>> reward = {{custom_cost}};
    // done
    bool ed_ep = false;
    if (valid_mappings_ > 1 && last_cost_ > custom_cost) {
      ed_ep = true;
    } else if (cost == 0) {
      //ed_ep = true;
    }   
    std::vector<std::vector<bool>> done = {{ed_ep}};
    last_cost_ = custom_cost;
    
    std::cout << "obs:[";
    for (int i = 0; i < 4; i++) std::cout << obs_[0][i] << " ";
    std::cout << "] reward:" << reward[0][0] << "\n";
    
    comm_->SendMlpStepResponse(obs_, reward, done, reward);

    //if (index == 50) exit(1);
    
    bool skip_datatype_bypass = false;
    if (status == Status::Success)
    {
      valid_mappings_++;
        
      if (best == 0 && cost > 0) {
        best = cost;  
      } else if (cost > 0) {
        best = std::min(best, cost);
      }  
        
      if (best_cost_ == 0)
        best_cost_ = cost;
      else
        best_cost_ = std::min(best_cost_, cost);
    }
    else if (status == Status::MappingConstructionFailure)
    {
      // Accelerate search by invalidating bad spaces.
      // ConstructMapping failure =>
      //   Combination of (IF, LP, S) is bad.
      //   Skip all DBs.
      skip_datatype_bypass = true;
    }
    else if (status == Status::EvalFailure)
    {
      // PreEval/Eval failure (capacity) =>
      //   Combination of (IF, DB) is bad.
      //   If all DBs cause Eval failure for an IF, then that IF is bad,
      //   no need to look at other LP, S combinations.
      eval_fail_count_++;
    }

    /*
    if (iterator_[unsigned(mapspace::Dimension::DatatypeBypass)] + 1 ==
        mapspace_->Size(mapspace::Dimension::DatatypeBypass))
    {
      if (eval_fail_count_ == mapspace_->Size(mapspace::Dimension::DatatypeBypass))
      {
        // All DBs failed eval for this combination of IF*LP*S. This means
        // this IF is bad. Skip to the next IF by fast-forwarding to the end of
        // this IF.
        iterator_[unsigned(mapspace::Dimension::Spatial)] =
          mapspace_->Size(mapspace::Dimension::Spatial) - 1;
        iterator_[unsigned(mapspace::Dimension::LoopPermutation)] =
          mapspace_->Size(mapspace::Dimension::LoopPermutation) - 1;
      }
      eval_fail_count_ = 0;
    }

    if (skip_datatype_bypass)
    {
      iterator_[unsigned(mapspace::Dimension::DatatypeBypass)] =
        mapspace_->Size(mapspace::Dimension::DatatypeBypass) - 1;
    }
    */
    samples++;
    if (skip_datatype_bypass){}
    comm_->RecvStepReq();
    std::cout << "decide next action:";
    unsigned action = comm_->actions[0][0];
    if (action == 2 * unsigned(mapspace::Dimension::IndexFactorization)) {
      iterator_[unsigned(mapspace::Dimension::IndexFactorization)] -= 1;
      std::cout << "decrease IF\n";
    } else if (action == 2 * unsigned(mapspace::Dimension::IndexFactorization) + 1) {
      iterator_[unsigned(mapspace::Dimension::IndexFactorization)] += 1;
      std::cout << "increase IF\n";
    } else if (action == 2 * unsigned(mapspace::Dimension::LoopPermutation)) {
      iterator_[unsigned(mapspace::Dimension::LoopPermutation)] -= 1;
      std::cout << "decrease P\n";
    } else if (action == 2 * unsigned(mapspace::Dimension::LoopPermutation) + 1) {
      iterator_[unsigned(mapspace::Dimension::LoopPermutation)] += 1;
      std::cout << "increase P\n";
    } else if (action == 2 * unsigned(mapspace::Dimension::DatatypeBypass)) {
      iterator_[unsigned(mapspace::Dimension::DatatypeBypass)] -= 1;
      std::cout << "decrease B\n";
    } else if (action == 2 * unsigned(mapspace::Dimension::DatatypeBypass) + 1) {
      iterator_[unsigned(mapspace::Dimension::DatatypeBypass)] += 1;
      std::cout << "increase B\n";
    } else if (action == 2 * unsigned(mapspace::Dimension::Spatial)) {
      iterator_[unsigned(mapspace::Dimension::Spatial)] -= 1;
      std::cout << "decrease S\n";
    } else if (action == 2 * unsigned(mapspace::Dimension::Spatial) + 1) {
      iterator_[unsigned(mapspace::Dimension::Spatial)] += 1;
      std::cout << "increase S\n";
    }
    // iterator_ element is unsigned, if 0-1=-1---> very large unsigned number
    for (auto dim : dim_order_) {
      if (iterator_[unsigned(dim)] == bound[unsigned(dim)]) {
        iterator_[unsigned(dim)] = 0;
        //iterator_[unsigned(dim)] = bound[unsigned(dim)] - 1;
        //iterator_[unsigned(dim)] = unsigned(ceil((bound[unsigned(dim)] - 1) * rand() / double(RAND_MAX)));
      } else if (iterator_[unsigned(dim)] > bound[unsigned(dim)]) {
        iterator_[unsigned(dim)] = 0;
        //iterator_[unsigned(dim)] = unsigned(ceil((bound[unsigned(dim)] - 1) * rand() / double(RAND_MAX)));
      }
    }
    std::cerr << "MAPPING ID: IF(" << iterator_[unsigned(mapspace::Dimension::IndexFactorization)]
              << ") P(" << iterator_[unsigned(mapspace::Dimension::LoopPermutation)]
              << ") B(" << iterator_[unsigned(mapspace::Dimension::DatatypeBypass)]
              << ") S(" << iterator_[unsigned(mapspace::Dimension::Spatial)]
              << ")" << std::endl;
    
    bool mapspace_remaining = true;//IncrementRecursive_();

    if (mapspace_remaining) //  && valid_mappings_ < search_size_)
    {
       state_ = State::Ready;
    }
    else
    {
      state_ = State::Terminated;
    }
  }
};

} // namespace search
