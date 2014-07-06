param n := read file as "1n" comment "#" use 1;
set A := {1 .. n};
set B := {n+1 .. 2*n};

param c[A*B] := read file as "n+" comment "#" skip 1;

var x[A*B] binary;

maximize weight:
    sum <i,j> in A*B: c[i,j]*x[i,j];

subto oneA: forall <j> in B do
    sum <i> in A: x[i,j] == 1;

subto oneB: forall <i> in A do
    sum <j> in B: x[i,j] == 1;
