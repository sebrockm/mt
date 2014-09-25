set term pdf
set output "kbplot.pdf"

set xlabel "Jobs"
set ylabel "Summe Zykluszeiten"

set key left top 

plot "< sort -k2nr ../kbsuperres.txt" using 2:($5/$7/100.0) title "Simulated Annealing" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($11/$13/100.0) title "Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($17/$19/100.0) title "DE-Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($23/$25/100.0) title "Gilmore Gomory" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($29/$31/100.0) title "Soylu H1" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($35/$37/100.0) title "Soylu H2" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($41/$43/100.0) title "Soylu H3" with lines
