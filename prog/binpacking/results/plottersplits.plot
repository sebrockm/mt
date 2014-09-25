set term pdf
set output "plotsplits.pdf"

set xlabel "Jobgruppen"
set ylabel "Jobgruppenteilungen"

set yrange [-0.1:6]

set key right top 

plot "randobj.txt" using 2:($12==0?1/0:($12-$2)) title "Teilungen" with lines
