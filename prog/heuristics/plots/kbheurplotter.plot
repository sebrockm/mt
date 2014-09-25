set term pdf
set output "kbheurplot.pdf"

set xlabel "Jobs"
set ylabel "Summe Zykluszeiten"

set key left top 

plot "< sort -k2nr ../kbsuperres.txt" using 2:($3/100) title "Simulated Annealing" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($9/100) title "Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($15/100) title "DE-Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($21/100) title "Gilmore Gomory" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($27/100) title "Soylu H1" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($33/100) title "Soylu H2" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($39/100) title "Soylu H3" with lines
