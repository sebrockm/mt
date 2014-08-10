# Minimiere die Zyklus-Zeiten
# Fuer jeden Zyklus wird die Zeit ueber das Maximum der gerade an der Anlage
# befindlichen Jobs bestimmt

#param file := "../data/test.dat";

# Anzahl Jobs und Machine
param m := read file as "1n" use 1 comment "#";
param n := read file as "2n" use 1 comment "#";
set Machines := {1 .. m};
set Jobs := {1 .. n};
set K := {1 .. n};
set T := {1 ..n+m-1};

# Zeiten einlesen
param times[Jobs*Machines] := read file as "n+" skip 1 use n comment "#";

# setup times inlesen
param s[Jobs*Jobs] := read file as "n+" skip 1+n use n comment "#";


# x_ik gibt an, ob Job i an Position k im Schedule steht
var x[Jobs*K] binary;
var c[T] real; #Cycle-Time zum Zeitpunkt t

# y_jh gibt an, ob Job j genau m Positionen vor Job h im Schedule liegt
var y[Jobs*Jobs] binary;


# Zykluszeit minimieren
minimize minCycle: sum <t> in T: c[t]
		 + sum <j,h> in Jobs*Jobs: s[j,h]*y[j,h];

#Jeder Job ist genau einer Position im Schedule zugewiesen
subto jobStart: forall <j> in Jobs do sum <k> in K: x[j,k] == 1;
						
#Jeder Position im Schedule ist genau ein Job zugewiesen
subto timeStart: forall <k> in K do sum <j> in Jobs: x[j,k] == 1;

#Bestimmung ueber Maximum der derzeit an der Anlage vorhandenen Jobs:
subto maxSynchron: forall <t> in T do
	forall <k> in {max(t-m+1,1) .. min(n,t)} do
		c[t] >= sum <j> in Jobs: x[j,k]*times[j,t-k+1];

#y bestimmen
subto needSetup: forall <j,h> in Jobs*Jobs do
	if m+1 <= n then
		forall <k> in {m+1 .. n} do
			y[j,h] >= x[j,k-m] + x[h,k] - 1
	end;
