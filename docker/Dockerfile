FROM ubuntu:16.04

ENV VER=00

RUN apt-get update && \
      apt-get -y install sudo

RUN sudo apt-get install -y software-properties-common
RUN sudo add-apt-repository -y ppa:deadsnakes/ppa && sudo apt-get update && sudo rm /usr/bin/python3 && sudo apt-get install -y python3.7
RUN ln -s /usr/bin/python3.7 /usr/bin/python3
RUN sudo apt-get install -y python3-dev python3-setuptools gcc \
         libtinfo-dev zlib1g-dev build-essential cmake vim\
         wget unzip libssl-dev scons\
         libconfig++-dev libboost-dev libboost-iostreams-dev libboost-serialization-dev libyaml-cpp-dev libncurses-dev libtinfo-dev libgpm-dev
RUN mkdir workspace

# install cmake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.20.3/cmake-3.20.3.tar.gz;tar -zxvf cmake-3.20.3.tar.gz
RUN cd cmake-3.20.3;./bootstrap;make -j4;make install
RUN export PATH=/workspace/cmake-3.20.3/bin:$PATH

# install libboost 1.64.0
ENV CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/include/python3.5/
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.64.0/source/boost_1_64_0.tar.gz;tar -xzvf boost_1_64_0.tar.gz
RUN cd boost_1_64_0 && ./bootstrap.sh && ./b2 && ./b2 install

# install timeloop + rl-cpp
RUN rm -r /usr/include/boost && rm /usr/lib/x86_64-linux-gnu/libboost*
#RUN cd workspace/timeloop && mkdir build
ENV CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/workspace/pytorch-cpp-rl/example/lib/spdlog/include:/workspace/pytorch-cpp-rl/example/lib/libzmq/include:/workspace/pytorch-cpp-rl/example/lib/msgpack-c/include
RUN wget https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-1.4.0%2Bcpu.zip && unzip libtorch-shared-with-deps-1.4.0+cpu.zip
#RUN cd /workspace/pytorch-cpp-rl && mkdir build && cd build && \
#    cmake -DCMAKE_PREFIX_PATH=/workspace/libtorch/ .. && \
#    mv example/libgym-client.so ../../timeloop/build/
ENV LD_LIBRARY_PATH=/workspace/timeloop/build:/boost_1_64_0/stage/lib/:$LD_LIBRARY_PATH
ENV PATH=/workspace/timeloop/bin:$PATH
