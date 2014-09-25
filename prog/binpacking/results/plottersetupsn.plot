set term pdf
set output "plotsetupsn.pdf"

set xlabel "Jobgruppen"
set ylabel "Anzahl Ressourcenwechsel"

set key right bottom 

plot "randobj.txt" using 2:($12==0?1/0:($12-8)) title "Ressourcenwechsel" with lines
