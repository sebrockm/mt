set term pdf
set output "coplot.pdf"

set xlabel "Jobs"
set ylabel "Anzahl Ressourcenwechsel"

set key left top 

plot "< sort -k2nr ../superres.txt" using 2:($8/$7) title "Simulated Annealing" with lines, \
    "" using 2:($14/$13) title "Non-Full-Schedule" with lines, \
    "" using 2:($20/$19) title "DE-Non-Full-Schedule" with lines, \
    "" using 2:($26/$25) title "Gilmore Gomory" with lines, \
    "" using 2:($32/$31) title "Soylu H1" with lines, \
    "" using 2:($38/$37) title "Soylu H2" with lines, \
    "" using 2:($44/$43) title "Soylu H3" with lines
