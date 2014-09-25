set term pdf
set output "kbplotmipdiffn.pdf"

set xlabel "Jobs"
set ylabel "Verbesserung durch die zweite Stufe"

set key right bottom 

plot \
    "< sort -k3nr kbobj.txt" using 3:($11-$11) notitle linecolor rgb "black" with lines, \
    "< sort -k3nr kbobj.txt" using 3:($11-$6) title "MIP var 10min, MIP fix 20min" with lines, \
    "< sort -k3nr kbobj.txt" using 3:($11-$7) title "MIP var 10min, Heuristik" with lines
