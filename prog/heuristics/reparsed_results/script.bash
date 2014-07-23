#!/bin/bash

for f in *.txt 
do 
    ../test ~/synmv/order_parser/orders_reparsed2/$f > $f
    ~/synmv/binpacking/mkampmey/gilmore_gomory/gg ~/synmv/order_parser/orders_reparsed2/$f >> $f
done

