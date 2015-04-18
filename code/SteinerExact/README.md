# Source code for the Branch and Smith\* methods

## Usage
Building from source requires C++11. To compile simply type
```
> make
```
This generates the executable SteinerExact that will run either of the two algorithms. Running with no arguments produces usage instructions:
```
> ./SteinerExact
Args:
Usage:
./SteinerExact [option list]
Where [option list] can contain the following:
  -method <Branch | SmithStar>   Default: Branch
  -d <int>  If no input is specified random terminals with this dimension will be generated. Default: 3
  -n <int>  If no input is specified this many random terminals will be generated. Default: 10
  -input <string>.stp  STP-formatted input file
  -seed <int>  Seed to be used for randomization. Default: 0
  -upperBound <float | MST | Heuristic>  Indicates the upper bound that will be used. Default: Heuristic
Options specific to Branch:
  -subsets <int>:<int|all>[,<int:int|all>,...]  Number of terminals and number of topologies to preprocess. Default: 5:all
Options specific to SmithStar:
  -brancher <SiteOrder|Furthest>  Branching strategy. Include terminals in the order they are presented or take furthest first (Default).
  -nodeComparator <BestFirst|DepthFirst|BreadthFirst>  Prioritization of BnB nodes. Default: BestFirst.
The following example will use Branch to solve a set of 12 terminals randomly distributed in a 4D hypercube:
./SteinerExact -method Branch -d 4 -n 12
```

### Output format of Branch method

The Branch method generates the following type of output
```
> ./SteinerExact -method Branch -n 10 -d 2
Arguments: -method Branch -n 10 -d 2
Random terminals in hyper-cube:
> terminal[0] = Point(0.2425783, 0.013469575)
> terminal[1] = Point(0.38313885, 0.41465269)
> terminal[2] = Point(0.067768973, 0.99312693)
> terminal[3] = Point(0.48430805, 0.76533777)
> terminal[4] = Point(0.031833815, 0.030935482)
> terminal[5] = Point(0.93264044, 0.88787953)
> terminal[6] = Point(0.59132973, 0.47877868)
> terminal[7] = Point(0.83335434, 0.1863352)
> terminal[8] = Point(0.73565271, 0.11505317)
> terminal[9] = Point(0.69865863, 0.35560411)
Using upper bound from heuristic: 2.5490487
Upper bound ratio: 0.98061187
Tolerance: 0.05
Time-stamp: Sat Apr 18 09:49:40 2015
CPU-time elapsed (in seconds): 0.000
Preprocessing phase ..
Preprocessing subsets of size 3 using Warren-Smith
Preprocessing subsets of size 4 using Warren-Smith
Preprocessing subsets of size 5 using Warren-Smith
Time-stamp: Sat Apr 18 09:49:40 2015
CPU-time elapsed (in seconds): 0.050
Bottleneck matrix:
   1   2   2   1   2   1   1   1   1
       1   1   1   1   1   2   2   1
           1   2   2   1   2   2   2
               2   1   1   2   2   1
                   3   1   2   1   2
                       1   1   1   1
                           2   2   1
                               1   1
                                   1


Generating equilateral points
Equilateral points with 2 terminals .. 44
Equilateral points with 3 terminals .. 26
Equilateral points with 4 terminals .. 40
Equilateral points with 5 terminals .. 58
Time-stamp: Sat Apr 18 09:49:40 2015
CPU-time elapsed (in seconds): 0.053
Concatenating equilateral points ..
5 4 1
> Ran WS on 360
5 3 2
> Ran WS on 590
4 4 2
> Ran WS on 260
4 3 3
New best full topology (((6,((7,8),9)),(2,(3,5))),((0,4),1)) Len=2.532916 Ratio=0.97402357)
Pruning: (3,5) (4,10) (5,19)
New best full topology (((6,((7,8),9)),((2,3),5)),((0,4),1)) Len=2.5194153 Ratio=0.96883194)
Pruning: (3,0) (4,1) (5,3)
> Ran WS on 263
Checked 1473 full topologies
Time-stamp: Sat Apr 18 09:49:40 2015
CPU-time elapsed (in seconds): 0.060
Steiner point coordinates:
point[10] = 0.38376878 0.41462961
point[11] = 0.49603398 0.75304503
point[12] = 0.48413144 0.76520701
point[13] = 0.43087134 0.20246672
point[14] = 0.23947076 0.022232238
point[15] = 0.21665546 0.050482341
point[16] = 0.72246554 0.25976028
point[17] = 0.72033521 0.25705758
Edges:
1 10
5 11
10 11
2 12
3 12
11 12
8 13
10 13
0 14
4 14
13 14
0 15
4 15
14 15
7 16
10 16
8 17
9 17
16 17
Evaluated_Level: 3 319
Evaluated_Level: 4 780
Evaluated_Level: 5 773
Evaluated_Level: 7 196
Evaluated_Level: 8 517
Evaluated_Level: 9 294
Evaluated_Level: 10 478
Evaluated_Nodes: 3357
Avg_Node_Size: 6.2186475
Took 0.060348 seconds
```
Of particular interest is last line specifying the CPU-time and the line 
```
New best full topology (((6,((7,8),9)),((2,3),5)),((0,4),1)) Len=2.5194153 Ratio=0.96883194)
```
which specifies the best found topology and its length. 


## Code overview

The main parts of the Branch algorithm are in `Branch/Branch.cpp`. The main parts of Smith\* are in `SmithStar/SmithStar.cpp`. The iterative procedure to obtain relative minimal trees is implemented in `PointPlacement.{h,cpp}` and is mostly taken from the Smith algorithm. 

