set term pdf
set output "linreg.pdf"

set xlabel "Jobgruppen"
set ylabel "Summe Zykluszeiten"

set key left top 

f(x)=a*x+b
g(x)=c*x+d

fit f(x) "< sort -k3nr kbobj.txt" using 3:10 via a,b
fit g(x) "< sort -k2nr ../../heuristics/kbres.txt" using 2:($3*1/$5)/100.0 via c,d

set xrange [3000:11000]
set yrange [0:16000]

plot f(x) title "lin reg DKA1", g(x) title "lin reg DKA2", f(x)-g(x) title "Differenz"
