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
Update compiler include path and linker path accordingly to ``C_INCLUDE_PATH``, ``CPLUS_INCLUDE_PATH`` and ``LD_LIBRARY_PATH``.
