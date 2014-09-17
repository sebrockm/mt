set term pdf
set output "plotn.pdf"

set xlabel "Jobs"
set ylabel "Summe Zykluszeiten"

set key left top 

plot "< sort -k3nr randobj.txt" using 3:5 title "MIP fix 20min" with lines, \
    "< sort -k3nr randobj.txt" using 3:6 title "MIP var 10min, MIP fix 20min" with lines, \
    "< sort -k3nr randobj.txt" using 3:7 title "MIP var 10min, Heuristik" with lines, \
    "< sort -k3nr randobj.txt" using 3:8 title "Heuristik, MIP fix 20min" with lines, \
    "< sort -k3nr randobj.txt" using 3:9 title "Heuristik, Heuristik" with lines, \
    "< sort -k3nr randobj.txt" using 3:10 title "Simulated Annealing" with lines, \
    "< sort -k3nr randobj.txt" using 3:4 title "LB" with lines
