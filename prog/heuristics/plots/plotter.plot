set term pdf
set output "plot.pdf"

set xlabel "Jobgruppen"
set ylabel "Summe Zykluszeiten"

set key left top 

plot "../superres.txt" using (column(0)+10):($5/$7/100) title "Simulated Annealing" with lines, \
    "" using (column(0)+10):($11/$13/100) title "Non-Full-Schedule" with lines, \
    "" using (column(0)+10):($17/$19/100) title "DE-Non-Full-Schedule" with lines, \
    "" using (column(0)+10):($23/$25/100) title "Gilmore Gomory" with lines, \
    "" using (column(0)+10):($29/$31/100) title "Soylu H1" with lines, \
    "" using (column(0)+10):($35/$37/100) title "Soylu H2" with lines, \
    "" using (column(0)+10):($41/$43/100) title "Soylu H3" with lines
