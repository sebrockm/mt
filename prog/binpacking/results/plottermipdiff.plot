set term pdf
set output "plotmipdiff.pdf"

set xlabel "Jobgruppen"
set ylabel "Verbesserung durch die zweite Stufe"

set xrange [10:40]

set key left bottom 

plot \
    "randobj.txt" using 2:($11-$11) notitle linecolor rgb "black" with lines, \
    "" using 2:($12==0?1/0:$11-$6) title "MIP var 10min, MIP fix 20min" with lines, \
    "" using 2:($12==0?1/0:$11-$7) title "MIP var 10min, Heuristik" with lines
