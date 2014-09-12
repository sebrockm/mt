#!/bin/bash

for f in ../../instances/bininstances/orders_reparsed/*.txt
do
    echo $f
    zimpl fine_planning_binvar.zpl -D file=$f.split -D binfile=$f.bins.dat
    cplex -c "re fine_planning_binvar.lp" "set time 300" "o" "dis sol var binpos*" | grep "binpos#" | tr "binpos#" " " > bla
    zimpl fine_planning_binfix.zpl -D file=$f.split -D binfile=$f.bins.dat
    cplex -c "re fine_planning_binfix.lp" "set time 600" "o" "dis sol obj" "dis sol bestb" > bla2
    echo -n "$f " >> var5fix10obj.txt
    echo -n "$f " >> var5fix10bound.txt
    tail bla2 | grep "Objective =" | sed -e "s/.*Objective =//" >> var5fix10obj.txt
    tail bla2 | grep "best bound =" | sed -e "s/.*best bound =//" >> var5fix10bound.txt
done
rm bla2 
