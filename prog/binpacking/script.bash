#!/bin/bash

for f in ../../instances/bininstances/test/*.txt
do
    echo $f
    #zimpl fine_planning_binvar.zpl -D file=$f
    #cplex -c "re fine_planning_binvar.lp" "set time 600" "o" "dis sol var binpos*" | grep "binpos#" | tr "binpos#" " " > bla
    zimpl fine_planning_binfix.zpl -D file=$f
    cplex -c "re fine_planning_binfix.lp" "set time 1800" "o" "dis sol obj" "dis sol bestb" > bla2
    echo -n "$f " >> obj2.txt
    echo -n "$f " >> bound2.txt
    tail bla2 | grep "Objective =" | sed -e "s/.*Objective =//" >> obj2.txt
    tail bla2 | grep "best bound =" | sed -e "s/.*best bound =//" >> bound2.txt
done
rm bla2 
