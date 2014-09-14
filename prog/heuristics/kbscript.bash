#!/bin/bash

for f in ../../instances/bininstances/orders_reparsed/*.txt
do
    echo $f
    echo -n "$f " >> kbres.txt
    ./test $f --groups --res >> kbres.txt 
done
