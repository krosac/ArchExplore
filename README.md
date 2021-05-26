# ArchExplore

[https://github.com/NVlabs/timeloop](https://github.com/NVlabs/timeloop)

testcase: [05-mapper-conv1d+oc-3level](https://github.com/Accelergy-Project/timeloop-accelergy-exercises/tree/master/exercises/timeloop/05-mapper-conv1d%2Boc-3level)

## Troubleshooting for Timeloop Installation

* libboost error
 
[https://github.com/NVlabs/timeloop/issues/3](https://github.com/NVlabs/timeloop/issues/3) solved by installing libboost 1.64

First remove **all** old boost headers and libs under ``/usr/include/``, ``/usr/local/include``, ``/usr/local/lib``, ``/usr/lib``, ``/usr/lib/x86_64-linux-gnu``. 
This step is necessary before installing new boost version because if there are multiple boost libs under ``LD_LIBRARY_PATH``, linker might choose the wrong one.
To make sure linker pick the new boost version, we must purge all old boost headers and libs.
For example,
```
sudo rm -r /usr/include/boost
sudo rm /usr/lib/x86_64-linux-gnu/libboost*
```
Then install boost 1.64.
```
wget https://boostorg.jfrog.io/artifactory/main/release/1.64.0/source/boost_1_64_0.tar.gz
tar -xzvf boost_1_64_0.tar.gz
cd boost_1_64_0
./bootstrap.sh
./b2
sudo ./b2 install
```
After these steps, ``scons -j4`` completes.
