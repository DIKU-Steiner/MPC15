#!/bin/bash

for f in Branch/*txt; do 
	basename=${f##*/}

	geosteinerLen=`grep length GeoSteiner/$basename | tail -n 1 | tr -s ' ' | cut -d ' ' -f 9 | cut -c1-9`

	branchLen=`grep Primal Branch/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f3 | cut -c1-9`;

	smithmLen=`grep record Smith_Martin/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f4 | cut -c1-9`;

	smithsLen=`grep Length Smith_Star/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f2| cut -c1-9`;

	smithLen=`grep record Smith/$basename | tail -n 1 | tr -s ' ' | cut -d' ' -f4 | cut -c1-9`;

	diffGeosteiner=$(echo "scale=4;$geosteinerLen-$geosteinerLen" | bc)
	diffBranch=$(echo "scale=4;$branchLen-$geosteinerLen" | bc)
	diffSmithM=$(echo "scale=4;$smithmLen-$geosteinerLen" | bc)
	diffSmithS=$(echo "scale=4;$smithsLen-$geosteinerLen" | bc)
	diffSmith=$(echo "scale=4;$smithLen-$geosteinerLen" | bc)

	echo "INSTANCE $basename"
	echo "LENGTH GeoSteiner   $geosteinerLen"
	echo "LENGTH Branch       $branchLen"
	echo "LENGTH Smith_Fixed  $smithmLen"
	echo "LENGTH Smith_Star   $smithsLen"
	echo "LENGTH Smith        $smithLen"

	echo "DIFFGEO GeoSteiner  $diffGeosteiner"
	echo "DIFFGEO Branch      $diffBranch"
	echo "DIFFGEO Smith_Fixed $diffSmithM"
	echo "DIFFGEO Smith_Star  $diffSmithS"
	echo "DIFFGEO Smith       $diffSmith"

done
