#!/bin/bash


sites=`grep DD $1 | wc -l`
dims=`grep DD $1 | head -n 1 | cut -f1 -d' ' | wc -c`
dims=$(expr $dims - 1)

echo  $sites
echo  $dims

grep DD $1 | cut -d' ' -f3-
