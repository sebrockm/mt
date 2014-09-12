#!/bin/bash
echo $#
if [ $# -eq 1 ]
then
    files=$1
else
    files=../order_parser/orders_reparsed/*.txt
fi

for f in $files
do
    t=$(date +%s%N)
    echo $t
    for i in $(seq 0 21)
    do
        zimpl binpacking -D partsline=$i -D file=$f
        cplex -c "read binpacking.lp" "optimize" "display solution variables *" > output$i.txt
        n=$(grep -ic "Integer optimal solution" output$i.txt)
        if [ $n -ge 1 ]
        then
            grep "x#" output$i.txt | tr "x#" " "> $f.bins.dat
            echo "#optimale Partition: s. part8.txt Nr. $(($i+1))" >> $f.bins.dat
            rm output$i.txt
            ./binsplitter 8 $f $f.bins.dat 
            break
        fi
        rm output$i.txt
    done
    echo "$f millis $((($(date +%s%N)-$t)/1000000))" >> binpacksplittimes.txt
done
