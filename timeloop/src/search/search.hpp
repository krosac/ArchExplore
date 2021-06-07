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

#include "mapspaces/mapspace-base.hpp"
#include <fstream>
#include <map>
#include "communicator.h"

using namespace gym_client;

namespace search
{

enum class Status
{
  Success,
  MappingConstructionFailure,
  EvalFailure
};

static std::string StatusToString(Status status) {
  if (status == Status::Success) {
    return "Success";
  } else if (status == Status::MappingConstructionFailure) {
    return "MappingConstructionFailure";
  } else if (status == Status::EvalFailure) {
    return "EvalFailure";
  } else {
    return "UnknownFailure";
  }
}

class SearchAlgorithm
{ 
 public:
  int index {0};
  std::map<std::string, int> status_count;
  std::ofstream os;
  
  SearchAlgorithm() {
    os.open("search-points.txt");
  }
  virtual ~SearchAlgorithm() {
    os.close();
    std::cout << "=========Status Stats==========\n";
    for (auto item : status_count) {
      std::cout << item.first << " : " << item.second << "\n";
    }
    std::cout << "Total : " << index << "\n";
  }
  virtual bool Next(mapspace::ID& mapping_id) = 0;
  virtual void Report(Status status, double cost = 0) = 0;
  void LogStats(Status status, double cost) {
    std::string status_str = StatusToString(status);
    os << index++ << ":" << status_str << ":" << cost << "\n";
    auto it = status_count.find(status_str);
    if (it == status_count.end()) {
      status_count[status_str] = 0;
    }
    status_count[status_str]++;
  }
};

} // namespace search

