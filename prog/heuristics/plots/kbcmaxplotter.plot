set term pdf
set output "kbcmaxplot.pdf"

set xlabel "Jobs"
set ylabel "Cmax"

set key left top 

plot "< sort -k2nr ../kbsuperres.txt" using 2:($8*5+$5/100/$7) title "Simulated Annealing" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($14*5+$11/100/$13) title "Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($20*5+$17/100/$19) title "DE-Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($26*5+$23/100/$25) title "Gilmore Gomory" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($32*5+$29/100/$31) title "Soylu H1" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($38*5+$35/100/$37) title "Soylu H2" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($44*5+$41/100/$43) title "Soylu H3" with lines
