set term pdf
set output "plotrel.pdf"

set xlabel "Jobgruppen"
set ylabel "Abstand zur LB in %"

set key left top 

plot "randobj.txt" using 2:(($5-$4)/$4*100) title "MIP fix 20min" with lines, \
    "" using 2:(($6-$4)/$4*100) title "MIP var 10min, MIP fix 20min" with lines, \
    "" using 2:(($7-$4)/$4*100) title "MIP var 10min, Heuristik" with lines, \
    "" using 2:(($8-$4)/$4*100) title "Heuristik, MIP fix 20min" with lines, \
    "" using 2:(($9-$4)/$4*100) title "Heuristik, Heuristik" with lines, \
    "" using 2:(($10-$4)/$4*100) title "Simulated Annealing" with lines
