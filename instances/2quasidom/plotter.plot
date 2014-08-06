set term pdf
set output "plot.pdf"

set xlabel "n"
set ylabel "Cmax"

plot "results.txt" using 1:(($2!=0?$2:1/0)-$7) title "CPLEX LB" with lines, \
    "results.txt" using 1:(($3!=0?$3:1/0)-$7) title "CPLEX UB" with lines, \
    "results.txt" using 1:($5-$7) title "Simulated Annealing" with lines, \
    "results.txt" using 1:($7-$7) title "Non-Full-Schedule" with lines, \
    "results.txt" using 1:($9-$7) title "DE-Non-Full-Schedule" with lines, \
    "results.txt" using 1:($11-$7) title "GG-Heuristik" with lines
