set term pdf
set output "plotm3.pdf"

#set xlabel "n"
set ylabel "Cmax"

set key right bottom

set xtics rotate

plot "resultsm3.dat" using (column(0)):2:xtic(1) title "Simulated Annealing" with lines, \
    "resultsm3.dat" using (column(0)):4:xtic(1) title "Non-Full-Schedule" with lines, \
    "resultsm3.dat" using (column(0)):6:xtic(1) title "DE-Non-Full-Schedule" with lines, \
    "resultsm3.dat" using (column(0)):8:xtic(1) title "GG-Heuristik" with lines, \
    "resultsm3.dat" using (column(0)):10:xtic(1) title "Soylu H1" with lines, \
    "resultsm3.dat" using (column(0)):12:xtic(1) title "Soylu H2" with lines, \
    "resultsm3.dat" using (column(0)):14:xtic(1) title "Soylu H3" with lines
