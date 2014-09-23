set term pdf
set output "plottime.pdf"

set xlabel "n"
set ylabel "ms"

plot "results.txt" using 2:4 title "Simulated Annealing" with lines, \
    "results.txt" using 2:6 title "Non-Full-Schedule" with lines, \
    "results.txt" using 2:8 title "DE-Non-Full-Schedule" with lines, \
    "results.txt" using 2:10 title "GG-Heuristik" with lines, \
    "results.txt" using 2:($12) title "Soylu H1" with lines, \
    "results.txt" using 2:($14) title "Soylu H2" with lines, \
    "results.txt" using 2:($16) title "Soylu H3" with lines
