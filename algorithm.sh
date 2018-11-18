#bin/bash

paramfile=$1
datafile=$2
resfile=$3
resfile2=$4
startline=$5
exactfile=$6
N=$7
T=$8
L=$9
m=${10}
ts=${11}
w=${12}
c=${13}
d=${14}

./main.out $datafile  $paramfile $resfile $resfile2 $startline $L $m $ts $w $c $d > main.result
./tongji/tongji.sh $exactfile $resfile $resfile2
