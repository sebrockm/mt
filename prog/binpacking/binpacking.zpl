#param file := "families.dat";
param partsfile := "part8.txt";
#param partsline := read "line" as "1n" use 1;

param n := read file as "1n" use 1 comment "#";
param m := 8;

set Items := {1 .. n};
set Bins := {1 .. m};

param a[Items] := read file as "n+" skip 1 use n comment "#";
param B := 1/m * sum <i> in Items: a[i];
param parts[Bins] := read partsfile as "n+" skip partsline use 1 default 0;
param binsizes[<i> in Bins] := ceil(B) * parts[i];

var x[Items*Bins] binary; 

maximize stupidfunc: sum <i> in Items: a[i] * x[i,1];

subto packAll: forall <i> in Items do
    sum <j> in Bins: x[i,j] == 1;

subto capacity: forall <j> in Bins do
    sum <i> in Items: a[i] * x[i,j] <= binsizes[j];

