set term pdf
set output "plotrel.pdf"

set xlabel "n"
set ylabel "Abweichung von Gilmore Gomory in %"

set yrange [-0.1:30]

set key right top

plot \
    "results.txt" using 2:($3/$9-1)*100 title "Simulated Annealing" with lines, \
    "results.txt" using 2:($5/$9-1)*100 title "Non-Full-Schedule" with lines, \
    "results.txt" using 2:($7/$9-1)*100 title "DE-Non-Full-Schedule" with lines, \
    "results.txt" using 2:($9/$9-1)*100 title "GG-Heuristik" with lines, \
    "results.txt" using 2:($11/$9-1)*100 title "Soylu H1" with lines, \
    "results.txt" using 2:($13/$9-1)*100 title "Soylu H2" with lines, \
    "results.txt" using 2:($15/$9-1)*100 title "Soylu H3" with lines
