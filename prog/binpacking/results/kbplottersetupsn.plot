set term pdf
set output "kbplotsetupsn.pdf"

set xlabel "Jobs"
set ylabel "Summe Rüstkosten"

set key right bottom 

plot "< sort -k3nr kbobj.txt" using 3:($12-8)*5 title "Rüstkosten" with lines
