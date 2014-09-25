set term pdf
set output "kbdiffplot.pdf"

set xlabel "Jobs"
set ylabel "Differenz zwischen nach und vor der Korrektur"

set key left bottom 

plot "< sort -k2nr ../kbsuperres.txt" using 2:(($5-$3)/$7/100.0) title "Simulated Annealing" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:(($11-$9)/$13/100.0) title "Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:(($17-$15)/$19/100.0) title "DE-Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:(($23-$21)/$25/100.0) title "Gilmore Gomory" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:(($29-$27)/$31/100.0) title "Soylu H1" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:(($35-$33)/$37/100.0) title "Soylu H2" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:(($41-$39)/$43/100.0) title "Soylu H3" with lines
