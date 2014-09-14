#!/bin/bash

for f in ../../instances/bininstances/orders_reparsed/*.txt
do
    echo $f
    ../local_bin_search/test $f.split $f.bins.dat > bla
    zimpl fine_planning_binfix.zpl -D file=$f.split -D binfile=$f.bins.dat
    cplex -c "re fine_planning_binfix.lp" "set time 600" "o" "dis sol obj" "dis sol bestb" > bla2
    echo -n "$f " >> greedyfix10obj.txt
    echo -n "$f " >> greedyfix10bound.txt
    tail bla2 | grep "Objective =" | sed -e "s/.*Objective =//" >> greedyfix10obj.txt
    tail bla2 | grep "best bound =" | sed -e "s/.*best bound =//" >> greedyfix10bound.txt
done
rm bla2 
