#!/bin/bash

for f in ../../instances/bininstances/rand_orders/order*.txt
do
    echo $f
    zimpl fine_planning_binvar.zpl -D file=$f.split -D binfile=$f.bins.dat
    cplex -c "re fine_planning_binvar.lp" "set time 300" "o" "dis sol obj" > bla2
    echo -n "$f " >> var5obj.txt
    tail bla2 | grep "Objective =" | sed -e "s/.*Objective =//" >> var5obj.txt
done
rm bla2 
