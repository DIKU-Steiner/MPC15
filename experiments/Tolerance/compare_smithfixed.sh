#!/bin/bash

for f in SmithFixed_t0.00001/*txt; do 
	basename=${f##*/}

	smithm1Time=`grep Took SmithFixed_t0.1/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f5`;
	smithm2Time=`grep Took SmithFixed_t0.01/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f5`;
	smithm3Time=`grep Took SmithFixed_t0.001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f5`;
	smithm4Time=`grep Took SmithFixed_t0.0001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f5`;
	smithm5Time=`grep Took SmithFixed_t0.00001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f5`;

	smithm1Len=`grep record SmithFixed_t0.1/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f4 | cut -c1-9`;
	smithm2Len=`grep record SmithFixed_t0.01/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f4 | cut -c1-9`;
	smithm3Len=`grep record SmithFixed_t0.001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f4 | cut -c1-9`;
	smithm4Len=`grep record SmithFixed_t0.0001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f4 | cut -c1-9`;
	smithm5Len=`grep record SmithFixed_t0.00001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f4 | cut -c1-9`;

	diff1=$(echo "scale=4;$smithm1Len-$smithm5Len" | bc)
	diff2=$(echo "scale=4;$smithm2Len-$smithm5Len" | bc)
	diff3=$(echo "scale=4;$smithm3Len-$smithm5Len" | bc)
	diff4=$(echo "scale=4;$smithm4Len-$smithm5Len" | bc)
	diff5=$(echo "scale=4;$smithm5Len-$smithm5Len" | bc)

	echo "INSTANCE $basename"
	echo "LENGTH Smith_Fixed_t0.1      $smithm1Len"
	echo "LENGTH Smith_Fixed_t0.01     $smithm2Len"
	echo "LENGTH Smith_Fixed_t0.001    $smithm3Len"
	echo "LENGTH Smith_Fixed_t0.0001   $smithm4Len"
	echo "LENGTH Smith_Fixed_t0.00001  $smithm5Len"

	echo "DIFF Smith_Fixed_t0.1      $diff1"
	echo "DIFF Smith_Fixed_t0.01     $diff2"
	echo "DIFF Smith_Fixed_t0.001    $diff3"
	echo "DIFF Smith_Fixed_t0.0001   $diff4"
	echo "DIFF Smith_Fixed_t0.00001  $diff5"

	echo "TIME Smith_Fixed_t0.1      $smithm1Time"
	echo "TIME Smith_Fixed_t0.01     $smithm2Time"
	echo "TIME Smith_Fixed_t0.001    $smithm3Time"
	echo "TIME Smith_Fixed_t0.0001   $smithm4Time"
	echo "TIME Smith_Fixed_t0.00001  $smithm5Time"

done
