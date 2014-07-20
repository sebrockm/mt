param file := "bench_cmax_n500_m2.dat1";

# Anzahl Jobs und Machine
param m := read file as "1n" use 1 comment "#";
param n := read file as "2n" use 1 comment "#";
set Machines := {1 .. m};
set Jobs := {1 .. n};
set K := {1 .. n};
set T := {1 ..n+m-1};

# Zeiten einlesen
param times[Jobs*Machines] := read file as "n+" skip 1 use n comment "#";

# x gibt an, ob Job i an Position k im Schedule steht
var x[Jobs*K] binary;
var c[T] real; #Cycle-Time zum Zeitpunkt t

# Zykluszeit minimieren
minimize minCycle: sum <t> in T: c[t];

#Jeder Job ist genau einer Position im Schedule zugewiesen
subto jobStart: forall <j> in Jobs do sum <k> in K: x[j,k] == 1;
						
#Jeder Position im Schedule ist genau ein Job zugewiesen
subto timeStart: forall <k> in K do sum <j> in Jobs: x[j,k] == 1;

#Bestimmung ueber Maximum der derzeit an der Anlage vorhandenen Jobs:
subto maxSynchron: forall <t> in T do
	forall <k> in {max(t-m+1,1) .. min(n,t)} do
		c[t] >= sum <j> in Jobs: x[j,k]*times[j,t-k+1];
