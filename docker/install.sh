set -e
set -u
set -o pipefail

mkdir timeloop/build
export CPLUS_INCLUDE_PATH=/workspace/pytorch-cpp-rl/example/lib/spdlog/include:/workspace/pytorch-cpp-rl/example/lib/libzmq/include:/workspace/pytorch-cpp-rl/example/lib/msgpack-c/include
wget https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-1.4.0%2Bcpu.zip
unzip libtorch-shared-with-deps-1.4.0+cpu.zip
cd pytorch-cpp-rl
mkdir build;cd build
cmake -DCMAKE_PREFIX_PATH=/workspace/libtorch/ ..
mv example/libgym-client.so ../../timeloop/build/

export LD_LIBRARY_PATH=/workspace/timeloop/build:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/workspace/boost_1_64_0/stage/lib/:$LD_LIBRARY_PATH