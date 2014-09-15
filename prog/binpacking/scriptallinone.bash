#!/bin/bash

for f in $1
do
    #binfix
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


    #binvar binfix
    zimpl fine_planning_binvar.zpl -D file=$f.split -D binfile=$f.bins.dat
    cplex -c "re fine_planning_binvar.lp" "set time 300" "o" "dis sol var binpos*" | grep "binpos#" | tr "binpos#" " " > bla
    zimpl fine_planning_binfix.zpl -D file=$f.split -D binfile=$f.bins.dat
    cplex -c "re fine_planning_binfix.lp" "set time 600" "o" "dis sol obj" "dis sol bestb" > bla2
    echo -n "$f " >> var5fix10obj.txt
    echo -n "$f " >> var5fix10bound.txt
    tail bla2 | grep "Objective =" | sed -e "s/.*Objective =//" >> var5fix10obj.txt
    tail bla2 | grep "best bound =" | sed -e "s/.*best bound =//" >> var5fix10bound.txt

    #binvar greedy
    ../local_bin_search/test2 $f.split $f.bins.dat < bla > bla2
    echo -n "$f " >> var5greedyobj.txt
    cat bla2 >> var5greedyobj.txt


    #greedy binfix
    ../local_bin_search/test $f.split $f.bins.dat > bla
    zimpl fine_planning_binfix.zpl -D file=$f.split -D binfile=$f.bins.dat
    cplex -c "re fine_planning_binfix.lp" "set time 600" "o" "dis sol obj" "dis sol bestb" > bla2
    echo -n "$f " >> greedyfix10obj.txt
    echo -n "$f " >> greedyfix10bound.txt
    tail bla2 | grep "Objective =" | sed -e "s/.*Objective =//" >> greedyfix10obj.txt
    tail bla2 | grep "best bound =" | sed -e "s/.*best bound =//" >> greedyfix10bound.txt

    #greedy greedy
    ../local_bin_search/test2 $f.split $f.bins.dat < bla > bla2
    echo -n "$f " >> greedygreedyobj.txt
    cat bla2 >> greedygreedyobj.txt
    echo "" >> greedygreedyobj.txt

    #sa
    echo -n "$f " >> saobj.txt 
    ../local_bin_search/sa $f.split $f.bins.dat >> saobj.txt

    #bound
    echo -n "$f " >> allbound.txt 
    ../local_bin_search/bound $f.split $f.bins.dat >> allbound.txt 
    echo "" >> allbound.txt
done
rm bla bla2
