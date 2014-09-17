set term pdf
set output "kbplot.pdf"

set xlabel "Jobgruppen"
set ylabel "Summe Zykluszeiten"

set key left top 

plot "< sort -k2nr kbobj.txt" using 2:5 title "MIP fix 20min" with lines, \
    "< sort -k2nr kbobj.txt" using 2:6 title "MIP var 10min, MIP fix 20min" with lines, \
    "< sort -k2nr kbobj.txt" using 2:7 title "MIP var 10min, Heuristik" with lines, \
    "< sort -k2nr kbobj.txt" using 2:8 title "Heuristik, MIP fix 20min" with lines, \
    "< sort -k2nr kbobj.txt" using 2:9 title "Heuristik, Heuristik" with lines, \
    "< sort -k2nr kbobj.txt" using 2:10 title "Simulated Annealing" with lines, \
    "< sort -k2nr kbobj.txt" using 2:4 title "LB" with lines
