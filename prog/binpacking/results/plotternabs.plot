set term pdf
set output "plotnabs.pdf"

set xlabel "Jobs"
set ylabel "Abstand zur LB"

set key left top

plot "< sort -k3nr randobj.txt" using 3:($5-$4) title "MIP fix 20min" with lines, \
    "< sort -k3nr randobj.txt" using 3:($6-$4) title "MIP var 10min, MIP fix 20min" with lines, \
    "< sort -k3nr randobj.txt" using 3:($7-$4) title "MIP var 10min, Heuristik" with lines, \
    "< sort -k3nr randobj.txt" using 3:($8-$4) title "Heuristik, MIP fix 20min" with lines, \
    "< sort -k3nr randobj.txt" using 3:($9-$4) title "Heuristik, Heuristik" with lines, \
    "< sort -k3nr randobj.txt" using 3:($10-$4) title "Simulated Annealing" with lines

