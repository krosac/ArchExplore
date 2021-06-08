mkdir timeloop/build
cd pytorch-cpp-rl
mkdir build;cd build
cmake -DCMAKE_PREFIX_PATH=/libtorch/ ..
make -j4
mv example/libgym-client.so ../../timeloop/build/
cd ..
