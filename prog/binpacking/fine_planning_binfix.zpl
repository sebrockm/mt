#param file := "../families.dat";
param binfile := file + ".bins.dat";
param n := read file as "1n" use 1 comment "#";
param p := 0;

set praeJobgroups := {1 .. n};
set Bins := {1 ..8};

param index[praeJobgroups] := read binfile as "1n" use n;
param tmp[praeJobgroups] := read binfile as "2n" use n;
param praebin[<i> in praeJobgroups] := sum <j> in praeJobgroups with i == index[j]: tmp[j];

set Jobgroups := {<i> in praeJobgroups with praebin[i] > 8*p and praebin[i] <= 8*(p+1)};
#set Jobgroups := {<i> in praeJobgroups with praebin[i] mod 3 == p};

param test[Bins] := <1> 8, <2> 7, <3> 3, <4> 5, <5> 2, <6> 1, <7> 4, <8> 6;

param bin[<i> in Jobgroups] := test[praebin[i] - 8*p];
#param bin[<i> in Jobgroups] := ceil(praebin[i] / 3);

param groupsize[praeJobgroups] := read file as "n+" skip 1 use n comment "#";
param B := (sum <i> in Jobgroups: groupsize[i]) / 8; 

param p1[praeJobgroups] := read file as "1n" skip 1+2*n use n comment "#";
param p2[praeJobgroups] := read file as "2n" skip 1+2*n use n comment "#";

param groupsPerBin[<b> in Bins] := sum <j> in Jobgroups with b == bin[j]: 1;

#i before j
var before[<i,j> in Jobgroups*Jobgroups with bin[i] == bin[j] and i != j] binary priority B-groupsPerBin[bin[i]];  

var overlay[<i,j> in Jobgroups*Jobgroups with bin[i] mod 8 == bin[j]-1] real >= 0 <= min(groupsize[i],groupsize[j]);

#indicator
var y[<i,j> in Jobgroups*Jobgroups with bin[i] mod 8 == bin[j]-1 or bin[j] mod 8 == bin[i]-1] binary;
var z[<i,j> in Jobgroups*Jobgroups with bin[i] mod 8 == bin[j]-1 or bin[j] mod 8 == bin[i]-1] binary;

var groupStart[<i> in Jobgroups] real >= 0 <= B - groupsize[i];


minimize makespan: 
    sum <i,j> in Jobgroups*Jobgroups with bin[i] mod 8 == bin[j]-1: 
        overlay[i,j] * max(p2[i], p1[j]);


subto symmetrieBefore: forall <i,j> in Jobgroups*Jobgroups with bin[i] == bin[j] and i < j do
    before[i,j] + before[j,i] == 1;

subto reflexivBefore: forall <i,j,k> in Jobgroups*Jobgroups*Jobgroups with bin[i] == bin[j] and bin[j] == bin[k] and i != j and j != k and i != k do
    before[i,j] + before[j,k] <= before[i,k] + 1;
    
#subto makeGroupStart: forall <i> in Jobgroups do
    #sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] == sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k];


# y[i,j] == 1 <=> sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] <= sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k]
subto makeY1: forall <i,j> in Jobgroups*Jobgroups with bin[i] == bin[j]-1 or bin[j] == bin[i]-1 do
    sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + 0.001 <= B * y[i,j];
subto makeY1spec: forall <i,j> in Jobgroups*Jobgroups with bin[i] == 8 and bin[j] == 1 do
    sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - 1 + 0.001 <= B * y[i,j];
subto makeY1spec2: forall <i,j> in Jobgroups*Jobgroups with bin[j] == 8 and bin[i] == 1 do
    sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] + 1 - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + 0.001 <= B * y[i,j];

subto makeY2: forall <i,j> in Jobgroups*Jobgroups with bin[i] == bin[j]-1 or bin[j] == bin[i]-1 do
    sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] <= B * (1-y[i,j]);
subto makeY2spec: forall <i,j> in Jobgroups*Jobgroups with bin[i] == 8 and bin[j] == 1 do
    sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + 1 - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] <= B * (1-y[i,j]);
subto makeY2spec2: forall <i,j> in Jobgroups*Jobgroups with bin[j] == 8 and bin[i] == 1 do
    sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - 1 <= B * (1-y[i,j]);


# z[i,j] == 1 <=> sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + groupsize[i] >= sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] + groupsize[j]
subto makeZ1: forall <i,j> in Jobgroups*Jobgroups with bin[i] == bin[j]-1 or bin[j] == bin[i]-1 do
    sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + groupsize[i] - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - groupsize[j] + 0.001 <= B * z[i,j];
subto makeZ1spec: forall <i,j> in Jobgroups*Jobgroups with bin[i] == 8 and bin[j] == 1 do
    sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + groupsize[i] + 1 - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - groupsize[j] + 0.001 <= B * z[i,j];
subto makeZ1spec2: forall <i,j> in Jobgroups*Jobgroups with bin[j] == 8 and bin[i] == 1 do
    sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + groupsize[i] - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - groupsize[j] - 1 + 0.001 <= B * z[i,j];
    
subto makeZ2: forall <i,j> in Jobgroups*Jobgroups with bin[i] == bin[j]-1 or bin[j] == bin[i]-1 do
    sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] + groupsize[j] - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - groupsize[i] <= B * (1-z[i,j]);
subto makeZ2spec: forall <i,j> in Jobgroups*Jobgroups with bin[i] == 8 and bin[j] == 1 do
    sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] + groupsize[j] - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - groupsize[i] - 1 <= B * (1-z[i,j]);
subto makeZ2spec2: forall <i,j> in Jobgroups*Jobgroups with bin[j] == 8 and bin[i] == 1 do
    sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] + groupsize[j] + 1 - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - groupsize[i] <= B * (1-z[i,j]);


subto makeOverlay1: forall <i,j> in Jobgroups*Jobgroups with bin[i] == bin[j]-1 do
    overlay[i,j] >= sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + groupsize[i] - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - B*(1-y[i,j]+z[i,j]);
subto makeOverlay2: forall <i,j> in Jobgroups*Jobgroups with bin[i] == bin[j]-1 do
    overlay[i,j] >= sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] + groupsize[j] - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - B*(1-y[j,i]+z[j,i]);

subto makeOverlay1spec: forall <i,j> in Jobgroups*Jobgroups with bin[i] == 8 and bin[j] == 1 do
    overlay[i,j] >= sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + 1 + groupsize[i] - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - B*(1-y[i,j]+z[i,j]);
subto makeOverlay2spec: forall <i,j> in Jobgroups*Jobgroups with bin[i] == 8 and bin[j] == 1 do
    overlay[i,j] >= sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] + groupsize[j] - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - 1 - B*(1-y[j,i]+z[j,i]);


subto makeSurround1: forall <i,j> in Jobgroups*Jobgroups with bin[i] mod 8 == bin[j]-1 do
    overlay[i,j] >= groupsize[j] - B*(2-y[i,j]-z[i,j]);
subto makeSurround2: forall <i,j> in Jobgroups*Jobgroups with bin[i] mod 8 == bin[j]-1 do
    overlay[i,j] >= groupsize[i] - B*(2-y[j,i]-z[j,i]);
