set term pdf
set output "plot.pdf"

set xlabel "n"
set ylabel "Cmax"

set key right bottom

plot "results.txt" using 1:(($2!=0?$2:1/0)) title "CPLEX LB" with lines, \
    "results.txt" using 1:(($3!=0?$3:1/0)) title "CPLEX UB" with lines, \
    "results.txt" using 1:($5) title "Simulated Annealing" with lines, \
    "results.txt" using 1:($7) title "Non-Full-Schedule" linecolor rgb "black" with lines, \
    "results.txt" using 1:($9) title "DE-Non-Full-Schedule" with lines, \
    "results.txt" using 1:($11) title "GG-Heuristik" linecolor rgb "orange" with lines
