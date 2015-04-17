#!/bin/bash

for f in SmithStar_t0.0001/*txt; do 
	basename=${f##*/}

	time1=`grep Took SmithStar_t0.1/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2`;
	time2=`grep Took SmithStar_t0.01/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2`;
	time3=`grep Took SmithStar_t0.001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2`;
	time4=`grep Took SmithStar_t0.0001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2`;

	len1=`grep Length SmithStar_t0.1/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2 | cut -c1-9`;
	len2=`grep Length SmithStar_t0.01/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2 | cut -c1-9`;
	len3=`grep Length SmithStar_t0.001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2 | cut -c1-9`;
	len4=`grep Length SmithStar_t0.0001/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2 | cut -c1-9`;

	diff1=$(echo "scale=4;$len1-$len3" | bc)
	diff2=$(echo "scale=4;$len2-$len3" | bc)
	diff3=$(echo "scale=4;$len3-$len3" | bc)
	diff4=$(echo "scale=4;$len4-$len3" | bc)

	echo "INSTANCE $basename"
	echo "LENGTH SmithStar_t0.1      $len1"
	echo "LENGTH SmithStar_t0.01     $len2"
	echo "LENGTH SmithStar_t0.001    $len3"
	echo "LENGTH SmithStar_t0.0001   $len4"

	echo "DIFF SmithStar_t0.1      $diff1"
	echo "DIFF SmithStar_t0.01     $diff2"
	echo "DIFF SmithStar_t0.001    $diff3"
	echo "DIFF SmithStar_t0.0001   $diff4"

	echo "TIME SmithStar_t0.1      $time1"
	echo "TIME SmithStar_t0.01     $time2"
	echo "TIME SmithStar_t0.001    $time3"
	echo "TIME SmithStar_t0.0001   $time4"

done
