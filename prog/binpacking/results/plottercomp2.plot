set term pdf
set output "plotcomp2.pdf"

set xlabel "Jobgruppen"
set ylabel "Cmax bei s=1"

set key left top 

plot "comp.txt" using 2:($3==0?1/0:$3+$4-8) title "Heuristik, MIP fix 20min" with lines, \
    "" using 2:($6==0?1/0:$5/100+$7) title "H2" with lines
