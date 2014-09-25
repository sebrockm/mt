set term pdf
set output "kbcoplot.pdf"

set xlabel "Jobs"
set ylabel "Summe der Rüstkosten"

set key left top 

plot "< sort -k2nr ../kbsuperres.txt" using 2:($8/$7)*5 title "Simulated Annealing" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($14/$13)*5 title "Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($20/$19)*5 title "DE-Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($26/$25)*5 title "Gilmore Gomory" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($32/$31)*5 title "Soylu H1" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($38/$37)*5 title "Soylu H2" with lines, \
    "< sort -k2nr ../kbsuperres.txt" using 2:($44/$43)*5 title "Soylu H3" with lines
