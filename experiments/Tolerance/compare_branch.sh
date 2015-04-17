#!/bin/bash

for f in Branch_t0.00001/*txt; do 
	basename=${f##*/}

	time1=`grep Took Branch_t0.1/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2`;
	time2=`grep Took Branch_t0.01/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2`;
	time3=`grep Took Branch_t0.001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2`;
	time4=`grep Took Branch_t0.0001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2`;
	time5=`grep Took Branch_t0.00001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2`;

	len1=`grep Primal Branch_t0.1/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f3 | cut -c1-9`;
	len2=`grep Primal Branch_t0.01/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f3 | cut -c1-9`;
	len3=`grep Primal Branch_t0.001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f3 | cut -c1-9`;
	len4=`grep Primal Branch_t0.0001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f3 | cut -c1-9`;
	len5=`grep Primal Branch_t0.00001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f3 | cut -c1-9`;

	diff1=$(echo "scale=4;$len1-$len5" | bc)
	diff2=$(echo "scale=4;$len2-$len5" | bc)
	diff3=$(echo "scale=4;$len3-$len5" | bc)
	diff4=$(echo "scale=4;$len4-$len5" | bc)
	diff5=$(echo "scale=4;$len5-$len5" | bc)

	echo "INSTANCE $basename"
	echo "LENGTH Branch_t0.1      $len1"
	echo "LENGTH Branch_t0.01     $len2"
	echo "LENGTH Branch_t0.001    $len3"
	echo "LENGTH Branch_t0.0001   $len4"
	echo "LENGTH Branch_t0.00001  $len5"

	echo "DIFF Branch_t0.1      $diff1"
	echo "DIFF Branch_t0.01     $diff2"
	echo "DIFF Branch_t0.001    $diff3"
	echo "DIFF Branch_t0.0001   $diff4"
	echo "DIFF Branch_t0.00001  $diff5"

	echo "TIME Branch_t0.1      $time1"
	echo "TIME Branch_t0.01     $time2"
	echo "TIME Branch_t0.001    $time3"
	echo "TIME Branch_t0.0001   $time4"
	echo "TIME Branch_t0.00001  $time5"

done
