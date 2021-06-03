* pack gymclient as a .so and move to timeloop/build
* scons -j4 under timeloop/

```
# 
export CPLUS_INCLUDE_PATH=/home/tiandong/hw/cs259_s21/pytorch-cpp-rl/example/lib/spdlog/include:/home/tiandong/hw/cs259_s21/pytorch-cpp-rl/example/lib/libzmq/include:/home/tiandong/hw/cs259_s21/pytorch-cpp-rl/example/lib/msgpack-c/include
# executable loading libgym_client.so (move libgym_client.so to build/ first)
export LD_LIBRARY_PATH=/home/tiandong/hw/cs259_s21/timeloop/build:$LD_LIBRARY_PATH
```
