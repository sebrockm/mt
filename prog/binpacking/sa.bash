#!/bin/bash

for f in ../../instances/bininstances/orders_reparsed/*.txt
do
    echo $f
    echo -n "$f " >> saobj.txt 
    ../local_bin_search/sa $f.split $f.bins.dat >> saobj.txt
done
