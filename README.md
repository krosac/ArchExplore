# ArchExplore

[https://github.com/NVlabs/timeloop](https://github.com/NVlabs/timeloop)

## Troubleshooting for Timeloop Installation

* libboost error 
[https://github.com/NVlabs/timeloop/issues/3](https://github.com/NVlabs/timeloop/issues/3) solved by installing libboost 1.64
```
wget https://boostorg.jfrog.io/artifactory/main/release/1.64.0/source/boost_1_64_0.tar.gz
tar -xzvf boost_1_64_0.tar.gz
cd boost_1_64_0
./bootstrap.sh
./b2
```
Verify installed Boost version
```
dpkg -s libboost-dev | grep Version
```
