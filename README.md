# Faster Exact Algorithms for Computing Steiner Trees in Higher Dimensional Euclidean Spaces

Code and data files submitted for the Mathematical Programming Computation journal special issue in extension of the 11th DIMACS Implementation Challenge: Steiner Tree Problems. 

## Software usage

Source code for the Branch and Smith\* algorithms can be compiled by
```
> cd code/SteinerExact/
> make
```
and executed by either
```
> code/SteinerExact/SteinerExact -method Branch filename.stp
```
or 
```
> code/SteinerExact/SteinerExact -method SmithStar filename.stp
```
Note that compiling requires C++11 but no other dependencies.

Source code for the original and fixed code from Warren Smith can be compiled by running
```
> cd code/WarrenSmith/
> make
```
Note that the Smith implementation does not take STP files as input, so it must be converted before executing:
```
> experiments/stpToSmith.sh filename.stp | code/WarrenSmith/wds_smt_timing
```

## Experiments

All runs can be executed by going to the experiment folder and running `make`. For instance:
```bash
> cd experiments/Cube/Branch/
> make
```

