set term pdf
set output "plot.pdf"

set xlabel "Jobgruppen"
set ylabel "Summe Zykluszeiten"

set key left top 

plot "randobj.txt" using 2:($12==0?1/0:$5) title "MIP fix 20min" with lines, \
    "" using 2:($12==0?1/0:$6) title "MIP var 10min, MIP fix 20min" with lines, \
    "" using 2:($12==0?1/0:$7) title "MIP var 10min, Heuristik" with lines, \
    "" using 2:($12==0?1/0:$8) title "Heuristik, MIP fix 20min" with lines, \
    "" using 2:($12==0?1/0:$9) title "Heuristik, Heuristik" with lines, \
    "" using 2:($12==0?1/0:$10) title "Simulated Annealing" with lines, \
    "" using 2:($12==0?1/0:$4) title "LB" with lines
