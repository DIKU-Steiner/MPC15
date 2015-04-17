#!/bin/bash

function printWS {
	echo "$1"
	for f in $1/*txt; do 
		basename=${f##*/}
		branchLen=`grep Primal Branch/$basename | tail -n 1 | cut -d' ' -f3`;
		top=`grep topology $1/$basename | tail -n 1 | cut -d' ' -f4`;

		smithLen=`grep record Smith/$basename | tail -n 1 | cut -d' ' -f4`;
		epsilon1=$(echo "scale=4;$branchLen-$smithmLen" | bc)
		epsilon2=$(echo "scale=4;$branchLen-$smithLen" | bc)
		echo -e "$basename \t $branchLen \t $smithmLen \t $smithLen \t $epsilon1 \t $epsilon2"; 

	done
}
