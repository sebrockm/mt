#!/bin/bash

for f in ../../instances/bininstances/orders_reparsed/*.txt
do
    echo $f
    echo -n "$f " >> allbound.txt 
    ../local_bin_search/bound $f.split $f.bins.dat >> allbound.txt 
    echo "" >> allbound.txt
done
