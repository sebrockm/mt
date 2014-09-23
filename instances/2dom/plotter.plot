set term pdf
set output "plot.pdf"

set xlabel "n"
set ylabel "Cmax"

set key right bottom

plot "results.txt" using 2:3 title "Simulated Annealing" with lines, \
    "results.txt" using 2:5 title "Non-Full-Schedule" with lines, \
    "results.txt" using 2:7 title "DE-Non-Full-Schedule" with lines, \
    "results.txt" using 2:9 title "GG-Heuristik" with lines, \
    "results.txt" using 2:11 title "Soylu H1" with lines, \
    "results.txt" using 2:13 title "Soylu H2" with lines, \
    "results.txt" using 2:15 title "Soylu H3" with lines
