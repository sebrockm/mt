#!/bin/bash

for f in ../../instances/bininstances/orders_reparsed/*.txt
do
    echo $f
    echo "1 1" > bla
    echo "2 2" >> bla
    echo "3 3" >> bla
    echo "4 4" >> bla
    echo "5 5" >> bla
    echo "6 6" >> bla
    echo "7 7" >> bla
    echo "8 8" >> bla
    zimpl fine_planning_binfix.zpl -D file=$f.split -D binfile=$f.bins.dat
    cplex -c "re fine_planning_binfix.lp" "set time 600" "o" "dis sol obj" "dis sol bestb" > bla2
    echo -n "$f " >> fix10obj.txt
    echo -n "$f " >> fix10bound.txt
    tail bla2 | grep "Objective =" | sed -e "s/.*Objective =//" >> fix10obj.txt
    tail bla2 | grep "best bound =" | sed -e "s/.*best bound =//" >> fix10bound.txt
done
rm bla2 
