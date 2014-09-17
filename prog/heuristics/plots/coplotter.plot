set term pdf
set output "coplot.pdf"

set xlabel "Jobs"
set ylabel "Anzahl Rüstkosten"

set key left top 

plot "< sort -k2nr ../randres.txt" using 2:($6*1/$5) title "Simulated Annealing" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($10*1/$9) title "Non-Full-Schedule" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($14*1/$13) title "DE-Non-Full-Schedule" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($18*1/$17) title "Gilmore Gomory" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($22*1/$21) title "Soylu H1" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($26*1/$25) title "Soylu H2" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($30*1/$29) title "Soylu H3" with lines
