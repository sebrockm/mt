set term pdf
set output "kbplotcmaxn.pdf"

set xlabel "Jobs"
set ylabel "Cmax"

set key right bottom 

plot "< sort -k3nr kbobj.txt" using 3:($5+($12-8)*5) title "MIP fix 20min" with lines, \
    "< sort -k3nr kbobj.txt" using 3:($6+($12-8)*5) title "MIP var 10min, MIP fix 20min" with lines, \
    "< sort -k3nr kbobj.txt" using 3:($7+($12-8)*5) title "MIP var 10min, Heuristik" with lines, \
    "< sort -k3nr kbobj.txt" using 3:($8+($12-8)*5) title "Heuristik, MIP fix 20min" with lines, \
    "< sort -k3nr kbobj.txt" using 3:($9+($12-8)*5) title "Heuristik, Heuristik" with lines, \
    "< sort -k3nr kbobj.txt" using 3:($10+($12-8)*5) title "Simulated Annealing" with lines
