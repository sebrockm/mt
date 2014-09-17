set term pdf
set output "kbplot.pdf"

set xlabel "Jobs"
set ylabel "Summe Zykluszeiten"

set key left top 

plot "< sort -k2nr ../kbres.txt" using 2:($3*1/$5)/100.0 title "Simulated Annealing" with lines, \
    "< sort -k2nr ../kbres.txt" using 2:($7*1/$9)/100.0 title "Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbres.txt" using 2:($11*1/$13)/100.0 title "DE-Non-Full-Schedule" with lines, \
    "< sort -k2nr ../kbres.txt" using 2:($15*1/$17)/100.0 title "Gilmore Gomory" with lines, \
    "< sort -k2nr ../kbres.txt" using 2:($19*1/$21)/100.0 title "Soylu H1" with lines, \
    "< sort -k2nr ../kbres.txt" using 2:($23*1/$25)/100.0 title "Soylu H2" with lines, \
    "< sort -k2nr ../kbres.txt" using 2:($27*1/$29)/100.0 title "Soylu H3" with lines
