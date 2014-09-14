#!/bin/bash

for f in ../../instances/bininstances/orders_reparsed/*.txt
do
    echo $f
    ../local_bin_search/test $f.split $f.bins.dat | ../local_bin_search/test2 $f.split $f.bins.dat > bla2
    echo -n "$f " >> greedygreedyobj.txt
    cat bla2 >> greedygreedyobj.txt
    echo "" >> greedygreedyobj.txt
done
rm bla2 
