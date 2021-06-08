```
sudo docker run --rm --pid=host\
                     --mount src="$(pwd)"/..,target=/workspace,type=bind\
                     -w /workspace\
                     -e "CI_BUILD_HOME=/workspace"\
                     -e "CI_BUILD_USER=$(id -u -n)"\
                     -e "CI_BUILD_UID=$(id -u)"\
                     -e "CI_BUILD_GROUP=$(id -g -n)"\
                     -e "CI_BUILD_GID=$(id -g)"\
                     -h timeloop-rl-docker\
                     --name timeloop-rl\
                     -it --net=host\
                     timeloop-rl:1.0\
                     /bin/bash
```

```
mkdir timeloop/build
apt-get install wget unzip libssl-dev scons
apt-get libconfig++-dev libboost-dev libboost-iostreams-dev libboost-serialization-dev libyaml-cpp-dev libncurses-dev libtinfo-dev libgpm-dev
wget https://github.com/Kitware/CMake/releases/download/v3.20.3/cmake-3.20.3.tar.gz
tar -zxvf cmake-3.20.3.tar.gz
cd cmake-3.20.3
./bootstrap
make -j4
make install
export PATH=/workspace/cmake-3.20.3/bin:$PATH


export CPLUS_INCLUDE_PATH=/workspace/pytorch-cpp-rl/example/lib/spdlog/include:/workspace/pytorch-cpp-rl/example/lib/libzmq/include:/workspace/pytorch-cpp-rl/example/lib/msgpack-c/include
wget https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-1.4.0%2Bcpu.zip
unzip libtorch-shared-with-deps-1.4.0+cpu.zip
cd pytorch-cpp-rl
mkdir build;cd build
cmake -DCMAKE_PREFIX_PATH=/workspace/libtorch/ ..
mv example/libgym-client.so ../../timeloop/build/

export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/usr/include/python3.5/"
wget https://boostorg.jfrog.io/artifactory/main/release/1.64.0/source/boost_1_64_0.tar.gz
tar -xzvf boost_1_64_0.tar.gz
cd boost_1_64_0
./bootstrap.sh
./b2
./b2 install

export LD_LIBRARY_PATH=/workspace/timeloop/build:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/workspace/boost_1_64_0/stage/lib/:$LD_LIBRARY_PATH


cd /workspace/examples/05-mapper-conv1d+oc-3level
sh run.sh
ctrl+p ctrl+q
new terminal
sudo docker exec -it timeloop-rl ./pytorch-cpp-rl/build/example/gym_client
```
