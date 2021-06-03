
* Hybrid
```
[  0] Utilization = 1.00 | pJ/MACC =  366.917 | L2[WIO] P1 - L1[] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  280.519 | L2[WIO] P1 - L1[W] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  269.495 | L2[WIO] P1 - L1[] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  183.097 | L2[WIO] P1 - L1[W] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  161.025 | L2[WIO] P1 - L1[WO] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  150.001 | L2[WIO] P1 - L1[O] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   63.603 | L2[WIO] P1 - L1[WO] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   52.830 | L2[WIO] P8 - L1[W] P2 R3 - L0[IO] K32
[  0] Utilization = 1.00 | pJ/MACC =   51.035 | L2[WIO] P8 - L1[WI] P2 R3 - L0[IO] K32
[  0] Utilization = 1.00 | pJ/MACC =   48.463 | L2[WIO] K4 - L1[WI] P8 K8 - L0[WIO] P2 R3
[  0] Utilization = 1.00 | pJ/MACC =   46.826 | L2[WIO] K4 - L1[I] K8 P8 - L0[WIO] P2 R3
[  0] Utilization = 1.00 | pJ/MACC =   45.437 | L2[WIO] K2 - L1[I] K8 R3 - L0[WIO] P16 K2
[  0] Utilization = 1.00 | pJ/MACC =   44.814 | L2[WIO] P1 - L1[I] K8 P16 - L0[WIO] R3 K4
[  0] STATEMENT: 500 suboptimal mappings found since the last upgrade, terminating search.

===============================================
               BEGIN DIAGNOSTICS
-----------------------------------------------
Fail class: Capacity

  Level: RegisterFile
    Fail count: 1392
    Sample mapping that experienced this fail class:

      MainMemory [ Weights:96 Inputs:18 Outputs:512 ]
      -----------------------------------------------
      | for K in [0:2)

      GlobalBuffer [ Outputs:256 ]
      ----------------------------
      |   for K in [0:2)
      |     for P in [0:2)

      RegisterFile [ Weights:24 Inputs:10 Outputs:64 ]
      ------------------------------------------------
      |       for P in [0:8)
      |         for R in [0:3)
      |           for K in [0:8)

    Fail reason: mapped tile size 96 exceeds buffer capacity 64

-----------------------------------------------
                 END DIAGNOSTICS
===============================================

Summary stats for best mapping found by mapper:
  Utilization = 1.00 | pJ/MACC =   44.814
=========Status Stats==========
EvalFailure : 1392
Success : 43025
Total : 44417
```
* Random
```
[  0] Utilization = 1.00 | pJ/MACC =  366.917 | L2[WIO] P1 - L1[] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  280.519 | L2[WIO] P1 - L1[W] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  269.495 | L2[WIO] P1 - L1[] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  183.097 | L2[WIO] P1 - L1[W] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  161.025 | L2[WIO] P1 - L1[WO] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  150.001 | L2[WIO] P1 - L1[O] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   63.603 | L2[WIO] P1 - L1[WO] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   52.830 | L2[WIO] P8 - L1[W] P2 R3 - L0[IO] K32
[  0] Utilization = 1.00 | pJ/MACC =   51.035 | L2[WIO] P8 - L1[WI] P2 R3 - L0[IO] K32
[  0] Utilization = 1.00 | pJ/MACC =   48.463 | L2[WIO] K4 - L1[WI] P8 K8 - L0[WIO] P2 R3
[  0] Utilization = 1.00 | pJ/MACC =   48.015 | L2[WIO] P8 - L1[W] K4 - L0[WIO] P2 R3 K8
[  0] Utilization = 1.00 | pJ/MACC =   44.814 | L2[WIO] P1 - L1[I] K8 P4 - L0[WIO] P4 R3 K4
[  0] Utilization = 1.00 | pJ/MACC =   44.359 | L2[WIO] K4 - L1[I] P8 - L0[WIO] P2 R3 K8
[  0] Utilization = 1.00 | pJ/MACC =   43.909 | L2[WIO] K8 - L1[] K4 - L0[WIO] P16 R3
[  0] STATEMENT: 500 suboptimal mappings found since the last upgrade, terminating search.

===============================================
               BEGIN DIAGNOSTICS
-----------------------------------------------
Fail class: Capacity

  Level: RegisterFile
    Fail count: 3648
    Sample mapping that experienced this fail class:

      MainMemory [ Weights:96 Inputs:18 Outputs:512 ]
      -----------------------------------------------
      | for K in [0:2)

      GlobalBuffer [ Weights:48 Inputs:18 Outputs:256 ]
      -------------------------------------------------
      |   for R in [0:3)
      |     for K in [0:2)

      RegisterFile [ Inputs:16 Outputs:128 ]
      --------------------------------------
      |       for P in [0:16)
      |         for K in [0:8)

    Fail reason: mapped tile size 96 exceeds buffer capacity 64

-----------------------------------------------
                 END DIAGNOSTICS
===============================================

Summary stats for best mapping found by mapper:
  Utilization = 1.00 | pJ/MACC =   43.909
=========Status Stats==========
EvalFailure : 3648
Success : 52289
Total : 55937
```
* Random-pruned
```
[  0] Utilization = 1.00 | pJ/MACC =  366.917 | L2[WIO] P1 - L1[] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  280.519 | L2[WIO] P1 - L1[W] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  269.495 | L2[WIO] P1 - L1[] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  183.097 | L2[WIO] P1 - L1[W] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  161.025 | L2[WIO] P1 - L1[WO] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  150.001 | L2[WIO] P1 - L1[O] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   63.603 | L2[WIO] P1 - L1[WO] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   60.879 | L2[WIO] P8 - L1[WO] R3 P2 - L0[WI] K32
[  0] Utilization = 1.00 | pJ/MACC =   54.923 | L2[WIO] K4 - L1[W] P8 K8 - L0[IO] P2 R3
[  0] Utilization = 1.00 | pJ/MACC =   52.197 | L2[WIO] K4 - L1[W] P8 K8 - L0[WIO] P2 R3
[  0] Utilization = 1.00 | pJ/MACC =   51.189 | L2[WIO] K4 - L1[WI] P8 K8 - L0[IO] P2 R3
[  0] Utilization = 1.00 | pJ/MACC =   48.463 | L2[WIO] K4 - L1[WI] P8 K8 - L0[WIO] P2 R3
[  0] Utilization = 1.00 | pJ/MACC =   46.826 | L2[WIO] K4 - L1[I] K8 P8 - L0[WIO] P2 R3
[  0] Utilization = 1.00 | pJ/MACC =   45.437 | L2[WIO] K2 - L1[I] K8 R3 - L0[WIO] P16 K2
[  0] Utilization = 1.00 | pJ/MACC =   44.814 | L2[WIO] P1 - L1[I] K8 P16 - L0[WIO] R3 K4
[  0] Utilization = 1.00 | pJ/MACC =   44.359 | L2[WIO] P1 - L1[I] K4 P16 - L0[WIO] R3 K8
[  0] STATEMENT: 500 suboptimal mappings found since the last upgrade, terminating search.

===============================================
               BEGIN DIAGNOSTICS
-----------------------------------------------
Fail class: Capacity

  Level: RegisterFile
    Fail count: 2752
    Sample mapping that experienced this fail class:

      MainMemory [ Weights:96 Inputs:18 Outputs:512 ]
      -----------------------------------------------
      | for K in [0:2)
      |   for R in [0:3)

      GlobalBuffer [ Outputs:256 ]
      ----------------------------
      |     for P in [0:4)

      RegisterFile [ Weights:16 Inputs:4 Outputs:64 ]
      -----------------------------------------------
      |       for P in [0:4)
      |         for K in [0:16)

    Fail reason: mapped tile size 96 exceeds buffer capacity 64

-----------------------------------------------
                 END DIAGNOSTICS
===============================================

Summary stats for best mapping found by mapper:
  Utilization = 1.00 | pJ/MACC =   44.359
=========Status Stats==========
EvalFailure : 2752
Success : 82241
Total : 84993
```
* Exhaustive
```
[  0] Utilization = 1.00 | pJ/MACC =  366.917 | L2[WIO] P1 - L1[] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  280.519 | L2[WIO] P1 - L1[W] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  269.495 | L2[WIO] P1 - L1[] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  183.097 | L2[WIO] P1 - L1[W] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  161.025 | L2[WIO] P1 - L1[WO] P1 - L0[] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =  150.001 | L2[WIO] P1 - L1[O] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   63.603 | L2[WIO] P1 - L1[WO] P1 - L0[I] P16 R3 K32
[  0] Utilization = 1.00 | pJ/MACC =   43.909 | L2[WIO] P1 - L1[] K32 - L0[WIO] P16 R3
[  0] STATEMENT: 500 suboptimal mappings found since the last upgrade, terminating search.

===============================================
               BEGIN DIAGNOSTICS
-----------------------------------------------
Fail class: Capacity

  Level: RegisterFile
    Fail count: 9664
    Sample mapping that experienced this fail class:

      MainMemory [ Weights:96 Inputs:18 Outputs:512 ]
      -----------------------------------------------
      | for P in [0:1)

      GlobalBuffer [ Inputs:18 Outputs:512 ]
      --------------------------------------
      |   for K in [0:4)

      RegisterFile [ Weights:24 Inputs:18 Outputs:128 ]
      -------------------------------------------------
      |     for P in [0:16)
      |       for R in [0:3)
      |         for K in [0:8)

    Fail reason: mapped tile size 96 exceeds buffer capacity 64

-----------------------------------------------
                 END DIAGNOSTICS
===============================================

Summary stats for best mapping found by mapper:
  Utilization = 1.00 | pJ/MACC =   43.909
=========Status Stats==========
EvalFailure : 9664
Success : 24641
Total : 34305
```
