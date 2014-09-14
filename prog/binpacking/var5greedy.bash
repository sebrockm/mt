#!/bin/bash

for f in ../../instances/bininstances/orders_reparsed/*.txt
do
    echo $f
    zimpl fine_planning_binvar.zpl -D file=$f.split -D binfile=$f.bins.dat
    cplex -c "re fine_planning_binvar.lp" "set time 300" "o" "dis sol var binpos*" | grep "binpos#" | tr "binpos#" " " | ../local_bin_search/test2 $f.split $f.bins.dat > bla2
    echo -n "$f " >> var5greedyobj.txt
    cat bla2 >> var5greedyobj.txt
done
rm bla2 
