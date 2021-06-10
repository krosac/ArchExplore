# ArchExplore

This project aims to integrate RL into the search algorithms in Timeloop based on

* Timeloop: [https://github.com/NVlabs/timeloop](https://github.com/NVlabs/timeloop)

* Testcase: [05-mapper-conv1d+oc-3level](https://github.com/Accelergy-Project/timeloop-accelergy-exercises/tree/master/exercises/timeloop/05-mapper-conv1d%2Boc-3level)

* RL C++: [https://github.com/Omegastick/pytorch-cpp-rl](https://github.com/Omegastick/pytorch-cpp-rl)

This work integrates RL agent as a socket server at [https://github.com/krosac/ArchExplore/blob/main/timeloop/src/search/rl.hpp](https://github.com/krosac/ArchExplore/blob/main/timeloop/src/search/rl.hpp).

## Docker Build
```
cd docker
```
Install docker
```
sudo sh install_docker.sh
```
Build docker
```
sudo docker build -f Dockerfile --tag timeloop-rl:1.0 .
```
Run a new instance in docker 
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
Build pytorch-cpp-rl and Timeloop inside docker
```
sh build_pytorch-cpp-rl.sh
sh build_timeloop.sh
```

## Run Example
Inside docker
```
cd examples/05-mapper-conv1d+oc-3level
sh run.sh
```
Detach with ``ctrl+p`` followed by ``ctrl+q``

Open a new ternimal and launch RL agent client
```
cd /root/directory/of/this/repo/
sudo docker exec -it timeloop-rl ./pytorch-cpp-rl/build/example/gym_client
```
You can see Timeloop interacts with RL agent.

## Evaluation

##### Baseline 1 (05-mapper-conv1d+oc-3level)
```
Initializing Index Factorization subspace.
  Factorization options along problem dimension K = 21
  Factorization options along problem dimension R = 3
  Factorization options along problem dimension P = 15
Mapspace Dimension [IndexFactorization] Size: 945
Mapspace Dimension [LoopPermutation] Size: 36
Mapspace Dimension [Spatial] Size: 1
Mapspace Dimension [DatatypeBypass] Size: 64
Mapspace split! Per-split Mapping Dimension [IndexFactorization] Size: 945 Residue: 0
```

* Linear-pruned Search (search algorithms supported by Timeloop: [https://github.com/NVlabs/timeloop/blob/master/doc/mapper.md](https://github.com/NVlabs/timeloop/blob/master/doc/mapper.md))

```
[  0] Utilization = 1.00 | pJ/MACC =  366.917 | L2[WIO] P1 - L1[] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  280.519 | L2[WIO] P1 - L1[W] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  269.495 | L2[WIO] P1 - L1[] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  183.097 | L2[WIO] P1 - L1[W] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  161.025 | L2[WIO] P1 - L1[WO] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  150.001 | L2[WIO] P1 - L1[O] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   63.603 | L2[WIO] P1 - L1[WO] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   43.909 | L2[WIO] P1 - L1[] K32 - L0[WIO] P16 R3
[  0] STATEMENT: search algorithm is done, terminating search.

===============================================
               BEGIN DIAGNOSTICS
-----------------------------------------------
Fail class: Capacity

  Level: RegisterFile
    Fail count: 6704
    Sample mapping that experienced this fail class:

      MainMemory [ Weights:96 Inputs:18 Outputs:512 ]
      -----------------------------------------------
      | for P in [0:2)
      |   for R in [0:3)

      GlobalBuffer [ Outputs:256 ]
      ----------------------------
      |     for K in [0:2)

      RegisterFile [ Weights:16 Inputs:8 Outputs:128 ]
      ------------------------------------------------
      |       for P in [0:8)
      |         for K in [0:16)

    Fail reason: mapped tile size 96 exceeds buffer capacity 64

-----------------------------------------------
                 END DIAGNOSTICS
===============================================

Summary stats for best mapping found by mapper:
  Utilization = 1.00 | pJ/MACC =   43.909
=========Status Stats==========
EvalFailure : 2656
Success : 29409
Total : 32065

```
|![](https://github.com/krosac/ArchExplore/blob/main/images/linear-pruned.png)|![](https://github.com/krosac/ArchExplore/blob/main/images/linear-pruned_vc500.png)|
|:--:|:--:|
|victory condition=0|victory condition=500|

Quite random search results, since the cost (edp) is not keeping improving.

|![](https://github.com/krosac/ArchExplore/blob/main/images/05-cost.png)|![](https://github.com/krosac/ArchExplore/blob/main/images/05-invalids.png)|
|:--:|:--:|
|edp cost|invalid mappings evaluated during the search|

In this testcase, we can find that 
* RL keeps improving the edp even starting at a bad point
* RL results in less invalid mappings during the search (means more valid points wihtin limited time)


