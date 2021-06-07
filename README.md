# ArchExplore

This project will integrate RL into the search algorithms in Timeloop.

Timeloop: [https://github.com/NVlabs/timeloop](https://github.com/NVlabs/timeloop)

Testcase: [05-mapper-conv1d+oc-3level](https://github.com/Accelergy-Project/timeloop-accelergy-exercises/tree/master/exercises/timeloop/05-mapper-conv1d%2Boc-3level)

RL C++: [https://github.com/Omegastick/pytorch-cpp-rl](https://github.com/Omegastick/pytorch-cpp-rl)

This work integrates RL agent as a socket server at [https://github.com/krosac/ArchExplore/blob/main/timeloop/src/search/rl.hpp](https://github.com/krosac/ArchExplore/blob/main/timeloop/src/search/rl.hpp).

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
