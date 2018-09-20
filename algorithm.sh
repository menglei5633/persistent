#bin/bash

paramfile=$1
datafile=$2
resfile=$3
startline=$4
exactfile=$5
N=$6
T=$7
is=$8

./main.out $datafile  $paramfile $resfile $startline

if [ $is == "1" ]; then
	./exactPersistent/exactPersistent.out $N $T $startline $datafile $exactfile
fi
./tongji/tongji.sh $exactfile $resfile 
