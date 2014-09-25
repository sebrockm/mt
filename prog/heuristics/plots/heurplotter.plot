set term pdf
set output "heurplot.pdf"

set xlabel "Jobgruppen"
set ylabel "Summe Zykluszeiten"

set key left top 

plot "../superres.txt" using (column(0)+10):($3/100) title "Simulated Annealing" with lines, \
    "" using (column(0)+10):($9/100) title "Non-Full-Schedule" with lines, \
    "" using (column(0)+10):($15/100) title "DE-Non-Full-Schedule" with lines, \
    "" using (column(0)+10):($21/100) title "Gilmore Gomory" with lines, \
    "" using (column(0)+10):($27/100) title "Soylu H1" with lines, \
    "" using (column(0)+10):($33/100) title "Soylu H2" with lines, \
    "" using (column(0)+10):($39/100) title "Soylu H3" with lines
