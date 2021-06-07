#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <msgpack.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/ostream.h>

#include "requests.h"
#include "third_party/zmq.hpp"


union S {
    std::string s;
    std::vector<int64_t> v;
  };  
namespace gym_client
{
  
    
class Communicator
{
  public:
    std::vector<std::vector<float>> actions;
  
    // server
    Communicator(const std::string &url) {
      context = std::make_unique<zmq::context_t>(1);
      socket = std::make_unique<zmq::socket_t>(*context, ZMQ_PAIR);
      socket->bind(url.c_str());
    }
    void ConnectClient() {
      std::cout << "Connecting to client\n";
      msgpack::sbuffer buffer;
      msgpack::pack(buffer, "Connection Established");
      zmq::message_t message(buffer.size());
      std::memcpy(message.data(), buffer.data(), buffer.size());
      socket->send(message);
      std::cout << "Connected\n";
    }
    
    void Serve() {
      std::cout << "Serving\n"; 
      auto res = get_response<MakeParam>("make");
      std::cout << "make::env_name:" << res->env_name << " "
                << "num_envs:" << res->num_envs << "\n";
      SendMakeResponse("OK");
      SendInfoResponse("Discrete", {8}, "Discrete", {4});
      auto res_reset = get_response<ResetParam>("reset");
      std::cout << "reset::x:" << res_reset->x << "\n";
      while(true){};
    }
    
    void RecvStepReq() {
      std::cout << ">>>>> wait step req from client...\n";
      auto res = get_response<StepParam>("step");
      actions = res->actions;
      std::cout << "Step action shape: " << res->actions.size();
      if (res->actions.size() > 0) {
        std::cout << "x" << res->actions[0].size();
        for (size_t i = 0; i < res->actions.size(); i++) {
          std::cout << " #" << i << "th: [";
          for (size_t j = 0; j < res->actions[0].size(); j++) {
            std::cout << res->actions[i][j] << " ";
          }
          std::cout << "] ";
        }
      }
      std::cout << "\n";
    }
    
    void SendMlpResetResponse(std::vector<std::vector<float>> observation) {
      msgpack::sbuffer buffer;
      MlpResetResponse obs;
      obs.observation = observation;
      msgpack::pack(buffer, obs);
      zmq::message_t message(buffer.size());
      std::memcpy(message.data(), buffer.data(), buffer.size());
      socket->send(message);
    }
    
    void SendMakeResponse(std::string result) {
      msgpack::sbuffer buffer;
      MakeResponse param;
      param.result = result;
      msgpack::pack(buffer, param);
      zmq::message_t message(buffer.size());
      std::memcpy(message.data(), buffer.data(), buffer.size());
      socket->send(message);
    }
    
    void SendInfoResponse(
        std::string action_space_type,
        std::vector<int64_t> action_space_shape,
        std::string observation_space_type,
        std::vector<int64_t> observation_space_shape) {
      InfoResponse info;
      info.action_space_type = action_space_type;
      info.action_space_shape = action_space_shape;
      info.observation_space_type = observation_space_type;
      info.observation_space_shape = observation_space_shape;
      msgpack::sbuffer buffer;
      msgpack::pack(buffer, info);
      zmq::message_t message(buffer.size());
      std::memcpy(message.data(), buffer.data(), buffer.size());
      socket->send(message);
    }
    
    void SendMlpStepResponse(
        std::vector<std::vector<float>> observation,
        std::vector<std::vector<float>> reward,
        std::vector<std::vector<bool>> done,
        std::vector<std::vector<float>> real_reward) {
      MlpStepResponse step;
      step.observation = observation;
      step.reward = reward;
      step.done = done;
      step.real_reward = real_reward;
      msgpack::sbuffer buffer;
      msgpack::pack(buffer, step);
      zmq::message_t message(buffer.size());
      std::memcpy(message.data(), buffer.data(), buffer.size());
      socket->send(message);
    }
    
    ~Communicator();

    std::string get_raw_response() {
      // Receive message
      zmq::message_t zmq_msg;
      socket->recv(&zmq_msg);
      // Cast message to string
      std::string response = std::string(static_cast<char *>(zmq_msg.data()), zmq_msg.size());
      return response;
    }
    
    

    template <class T>
    std::shared_ptr<T> get_response(std::string method)
    {
        // Receive message
        zmq::message_t packed_msg;
        socket->recv(&packed_msg);

        // Desrialize message
        msgpack::object_handle object_handle = msgpack::unpack(static_cast<char *>(packed_msg.data()), packed_msg.size());
        msgpack::object object = object_handle.get();

        // Fill out response object
        auto param = std::make_shared<T>();
        Request<T> response(method, param);
        try
        {
            object.convert(response);
        }
        catch (...)
        {
            spdlog::error("Communication error: {}", object);
        }

        return response.param;
    }

    template <class T>
    void send_request(const Request<T> &request)
    {
        msgpack::sbuffer buffer;
        msgpack::pack(buffer, request);

        zmq::message_t message(buffer.size());
        std::memcpy(message.data(), buffer.data(), buffer.size());
        socket->send(message);
    }

  private:
    std::unique_ptr<zmq::context_t> context;
    std::unique_ptr<zmq::socket_t> socket;
};
}