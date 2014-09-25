set term pdf
set output "diffplot.pdf"

set xlabel "Jobgruppen"
set ylabel "Differenzen zwischen nach und vor der Korrektur"

set key left bottom

plot "../superres.txt" using (column(0)+10):(($5-$3)/$7/100) title "Simulated Annealing" with lines, \
    "" using (column(0)+10):(($11-$9)/$13/100) title "Non-Full-Schedule" with lines, \
    "" using (column(0)+10):(($17-$15)/$19/100) title "DE-Non-Full-Schedule" with lines, \
    "" using (column(0)+10):(($23-$21)/$25/100) title "Gilmore Gomory" with lines, \
    "" using (column(0)+10):(($29-$27)/$31/100) title "Soylu H1" with lines, \
    "" using (column(0)+10):(($35-$33)/$37/100) title "Soylu H2" with lines, \
    "" using (column(0)+10):(($41-$39)/$43/100) title "Soylu H3" with lines
