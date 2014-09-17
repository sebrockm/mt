set term pdf
set output "plotabs.pdf"

set xlabel "Jobgruppen"
set ylabel "Abstand zur LB"

set key left top

plot "randobj.txt" using 2:($5-$4) title "MIP fix 20min" with lines, \
    "randobj.txt" using 2:($6-$4) title "MIP var 10min, MIP fix 20min" with lines, \
    "randobj.txt" using 2:($7-$4) title "MIP var 10min, Heuristik" with lines, \
    "randobj.txt" using 2:($8-$4) title "Heuristik, MIP fix 20min" with lines, \
    "randobj.txt" using 2:($9-$4) title "Heuristik, Heuristik" with lines, \
    "randobj.txt" using 2:($10-$4) title "Simulated Annealing" with lines

