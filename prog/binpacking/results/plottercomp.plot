set term pdf
set output "plotcomp.pdf"

set xlabel "Jobgruppen"
set ylabel "Cmax bei s=0"

set key left top 

plot "comp.txt" using 2:($3==0?1/0:$3) title "Heuristik, MIP fix 20min" with lines, \
    "" using 2:($6==0?1/0:$5/100) title "H2" with lines
