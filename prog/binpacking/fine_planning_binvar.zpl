#param file := "../families.dat";
#param binfile := file + ".bins.dat";
param n := read file as "1n" use 1 comment "#";
param p := 0;

set praeJobgroups := {1 .. n};
set Bins := {1 ..8};

param index[praeJobgroups] := read binfile as "1n" use n;
param tmp[praeJobgroups] := read binfile as "2n" use n;
param praebin[<i> in praeJobgroups] := sum <j> in praeJobgroups with i == index[j]: tmp[j];

set Jobgroups := {<i> in praeJobgroups with praebin[i] > 8*p and praebin[i] <= 8*(p+1)};
#set Jobgroups := {<i> in praeJobgroups with praebin[i] mod 3 == p};

param bin[<i> in Jobgroups] := praebin[i] - 8*p;
#param bin[<i> in Jobgroups] := ceil(praebin[i] / 3);

param groupsize[praeJobgroups] := read file as "n+" skip 1 use n comment "#";
param B := (sum <i> in Jobgroups: groupsize[i]) / 8; 

param p1[praeJobgroups] := read file as "1n" skip 1+2*n use n comment "#";
param p2[praeJobgroups] := read file as "2n" skip 1+2*n use n comment "#";

param groupsPerBin[<b> in Bins] := sum <j> in Jobgroups with b == bin[j]: 1;

param epsilon := 0.001;
param M := B + epsilon;

#i before j
var before[<i,j> in Jobgroups*Jobgroups with bin[i] == bin[j] and i != j] binary priority B-groupsPerBin[bin[i]];  

var binpos[Bins*Bins] binary;

#b left to c, b on pos p, c on pos p%8+1
var next[Bins*Bins*Bins] binary;

var overlay[<i,j> in Jobgroups*Jobgroups with bin[i] != bin[j]] real >= 0 <= min(groupsize[i],groupsize[j]);

#indicator
var y[<i,j> in Jobgroups*Jobgroups with bin[i] != bin[j]] binary;
var z[<i,j> in Jobgroups*Jobgroups with bin[i] != bin[j]] binary;

#var groupStart[<i> in Jobgroups] real >= 0 <= B - groupsize[i];


minimize makespan: 
    sum <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j]: 
        overlay[i,j] * max(p2[i], p1[j]);

#subto a: binpos[8,1] == 1;
#subto b: binpos[2,2] == 1;
#subto c: binpos[3,3] == 1;
#subto d: binpos[4,4] == 1;
#subto e: binpos[5,5] == 1;
#subto f: binpos[6,6] == 1;
#subto g: binpos[1,7] == 1;
#subto h: binpos[7,8] == 1;

#subto i: before[1,11] == 1;
#subto j: before[2,12] == 1;
#subto k: before[2,6] == 1;
#subto l: before[12,6] == 1;
#subto m: before[8,3] == 1;
#subto n: before[8,14] == 1;
#subto o: before[3,14] == 1;

subto allOver: sum <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j]: overlay[i,j] == 8*B-1;

subto oneBin: forall <pi> in Bins do
    sum <b> in Bins: binpos[b,pi] == 1;

subto onePos: forall <b> in Bins do
    sum <pi> in Bins: binpos[b,pi] == 1;

subto linkBinposNext: forall <b,c,pi> in Bins*Bins*Bins do
    binpos[b,pi] + binpos[c,(pi mod 8)+1] <= next[b,c,pi] + 1;

subto linkBinposNextRev1: forall <b,c,pi> in Bins*Bins*Bins do
    next[b,c,pi] <= binpos[b,pi];

subto linkBinposNextRev2: forall <b,c,pi> in Bins*Bins*Bins do
    next[b,c,pi] <= binpos[c,(pi mod 8)+1];


subto symmetrieBefore: forall <i,j> in Jobgroups*Jobgroups with bin[i] == bin[j] and i < j do
    before[i,j] + before[j,i] == 1;

subto transitivBefore: forall <i,j,k> in Jobgroups*Jobgroups*Jobgroups with bin[i] == bin[j] and bin[j] == bin[k] and i != j and j != k and i != k do
    before[i,j] + before[j,k] <= before[i,k] + 1;
    


# y[i,j] == 1 <=> groupStart[i] <= groupStart[j]        i starts before j
subto makeY1: forall <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j] do
    sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - next[bin[i],bin[j],8] + epsilon <= M * (y[i,j]); 

subto makeY2: forall <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j] do
    sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + next[bin[i],bin[j],8] - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] <= M * (1-y[i,j]);


# z[i,j] == 1 <=> groupStart[i] + groupsize[i] >= groupStart[j] + groupsize[j]         i ends after j
subto makeZ1: forall <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j] do
    sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + groupsize[i] + next[bin[i],bin[j],8] - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - groupsize[j] + epsilon <= M * (z[i,j]);
    
subto makeZ2: forall <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j] do
    sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] + groupsize[j] - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - groupsize[i] - next[bin[i],bin[j],8] <= M * (1-z[i,j]);


# y[i,j] && !z[i,j]     i starts before j and ends before j
subto makeOverlay1: forall <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j] do
    overlay[i,j] >= sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] + next[bin[i],bin[j],8] + groupsize[i] - sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] - M*(1-y[i,j]+z[i,j]+1 - sum <pi> in Bins: next[bin[i],bin[j],pi]);

# !y[i,j] && z[i,j]     i starts after j and ends after j
subto makeOverlay2: forall <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j] do
    overlay[i,j] >= sum <k> in Jobgroups with bin[j] == bin[k] and j != k: before[k,j] * groupsize[k] + groupsize[j] - sum <k> in Jobgroups with bin[i] == bin[k] and i != k: before[k,i] * groupsize[k] - next[bin[i],bin[j],8] - M*(1-z[i,j]+y[i,j]+1 - sum <pi> in Bins: next[bin[i],bin[j],pi]);

# y[i,j] && z[i,j]      i starts before j and ends after j, i.e. i surrounds j
subto makeSurround1: forall <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j] do
    overlay[i,j] >= groupsize[j] - M*(2-y[i,j]-z[i,j]+1 - sum <pi> in Bins: next[bin[i],bin[j],pi]);

# !y[i,j] && !z[i,j]    i starts after j and ends before j, i.e. i is surrounded by j
subto makeSurround2: forall <i,j> in Jobgroups*Jobgroups with bin[i] != bin[j] do
    overlay[i,j] >= groupsize[i] - M*(y[i,j]+z[i,j]+1 - sum <pi> in Bins: next[bin[i],bin[j],pi]);
