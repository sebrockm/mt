set term pdf
set output "plottime.pdf"

set xlabel "n"
set ylabel "ms"

set key top left

plot "results.txt" using 1:6 title "Simulated Annealing" with lines, \
    "results.txt" using 1:8 title "Non-Full-Schedule" with lines, \
    "results.txt" using 1:10 title "DE-Non-Full-Schedule" with lines, \
    "results.txt" using 1:12 title "GG-Heuristik" with lines, \
    "results.txt" using 1:($14) title "Soylu H1" with lines, \
    "results.txt" using 1:($16) title "Soylu H2" with lines, \
    "results.txt" using 1:($18) title "Soylu H3" with lines
