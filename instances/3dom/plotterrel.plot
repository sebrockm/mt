set term pdf
set output "plotrel.pdf"

set xlabel "n"
set ylabel "Abweichung von Non-Full-Schedule in %"

set key right bottom

plot "results.txt" using 1:(($2!=0?$2:1/0)/$7-1)*100 title "CPLEX LB" with lines, \
    "results.txt" using 1:(($3!=0?$3:1/0)/$7-1)*100 title "CPLEX UB" with lines, \
    "results.txt" using 1:($5/$7-1)*100 title "Simulated Annealing" with lines, \
    "results.txt" using 1:($7/$7-1)*100 title "Non-Full-Schedule" linecolor rgb "black" with lines, \
    "results.txt" using 1:($9/$7-1)*100 title "DE-Non-Full-Schedule" with lines, \
    "results.txt" using 1:($11/$7-1)*100 title "GG-Heuristik" linecolor rgb "orange" with lines
