#!/bin/bash

for f in ../../instances/bininstances/rand_orders/*.txt
do
    echo $f
    echo -n "$f " >> randres.txt
    ./test $f --groups --res >> randres.txt 
done
