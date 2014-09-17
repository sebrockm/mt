set term pdf
set output "plotabs.pdf"

set xlabel "Jobs"
set ylabel "Abweichung von NFS in"

set key left top 

plot "< sort -k2nr ../randres.txt" using 2:($3/$5-$7/$9)/100 title "Simulated Annealing" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($7/$9-$7/$9)/100 title "Non-Full-Schedule" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($11/$13-$7/$9)/100 title "DE-Non-Full-Schedule" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($15/$17-$7/$9)/100 title "Gilmore Gomory" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($19/$21-$7/$9)/100 title "Soylu H1" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($23/$25-$7/$9)/100 title "Soylu H2" with lines, \
    "< sort -k2nr ../randres.txt" using 2:($27/$29-$7/$9)/100 title "Soylu H3" with lines
