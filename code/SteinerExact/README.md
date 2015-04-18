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



## Code overview

The main parts of the Branch algorithm are in `Branch/Branch.cpp`. 

The Warren Smith point placement is in `PointPlacement.{h,cpp}`. 

