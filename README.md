# Faster Exact Algorithms for Computing Steiner Trees in Higher Dimensional Euclidean Spaces


The Euclidean Steiner Minimal Tree (ESTP) problem seeks a network of minimal total edge length spanning a set of n terminal points while allowing for the insertion of additional points (Steiner points) to decrease the overall length of the network.

This repository contains 3 implementations (Branch, Smith\*, and Smith) for solving ESTP in any dimension to optimality. Typical point sets containing up to 20 terminals can be solved in 2D and up to 16 terminals in 5D. For detailed descriptions of these methods, or if used in published research, please see:

* [Faster Exact Algorithms for Computing Steiner Trees in Higher Dimensional Euclidean Spaces](http://dimacs11.cs.princeton.edu/workshop/FonsecaBrazilWinterZachariasen.pdf). R Fonseca, M Brazil, P Winter, M Zachariasen. 11th DIMACS Implementation Challenge on Steiner Tree Problems
* [How to find Steiner minimal trees in euclideand-space](http://link.springer.com/article/10.1007%2FBF01758756) W D Smith. Algorithmica (1992), Volume 7, Issue 1-6, pp 137-177

Code and data files are part of a submission for the Mathematical Programming Computation journal special issue in extension of the 11th DIMACS Implementation Challenge: Steiner Tree Problems. 

## Software usage

To compile and solve a Steiner tree problem (specified as an STP file) using first the Branch method and then the Smith\* method run:
```
> cd code/SteinerExact/
> make
> ./SteinerExact -method Branch -input filename.stp
> ./SteinerExact -method SmithStar -input filename.stp
```

Compiling and running the fixed version of Warren Smiths original algorithm can be done by
```
> cd code/WarrenSmith/
> make
> ./stpToSmith.sh filename.stp | ./wds_smt_timing
```

Further details about program compiling and usage is found in the READMEs under the `code` directory.

## Experiments

All runs can be executed by going to the experiment folder and running `make`. For instance:
```
> cd experiments/Cube/Branch/
> make
```
For futher details see the `experiments/README.md` file.

