set term pdf
set output "plottime.pdf"

set xlabel "n"
set ylabel "ms"

set key left top

plot "results.txt" using 1:6 title "Simulated Annealing" linecolor rgb "blue" with lines, \
    "results.txt" using 1:8 title "Non-Full-Schedule" linecolor rgb "black" with lines, \
    "results.txt" using 1:10 title "DE-Non-Full-Schedule" linecolor rgb "cyan" with lines, \
    "results.txt" using 1:12 title "GG-Heuristik" linecolor rgb "orange" with lines, \
    "results.txt" using 1:14 title "Soylu H1" with lines, \
    "results.txt" using 1:16 title "Soylu H2" with lines, \
    "results.txt" using 1:18 title "Soylu H3" with lines
