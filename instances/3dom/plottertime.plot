set term pdf
set output "plottime.pdf"

set xlabel "n"
set ylabel "ms"

plot "results.txt" using 1:6 title "Simulated Annealing" with lines, \
    "results.txt" using 1:8 title "Non-Full-Schedule" with lines, \
    "results.txt" using 1:10 title "DE-Non-Full-Schedule" with lines, \
    "results.txt" using 1:12 title "GG-Heuristik" with lines
