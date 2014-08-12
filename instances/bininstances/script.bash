#!/bin/bash

if [ $# -ne 1 ]
then
    echo "usage: $0 <N>"
    exit
fi

f=order_n$1_$RANDOM.txt

echo $1 > $f
echo "" >> $f

sum=0
for i in $(seq 1 $(($1-1)))
do
    tmp=$(($RANDOM%$(($1*5))+1)) 
    sum=$(($sum+$tmp))
    echo $tmp >> $f
done
echo $(($(($1/2*8))-$(($sum%8)))) >> $f

echo "" >> $f

for i in $(seq 1 $1)
do
    echo $(($RANDOM%10+1)) >> $f
done

echo "" >> $f

for i in $(seq 1 $1)
do
    N1=$(($RANDOM%150+50))
    N2=$(($RANDOM%150+50))
    echo "$(($N1/100)).$(($N1%100/10))$(($N1%10)) $(($N2/100)).$(($N2%100/10))$(($N2%10))" >> $f
done
