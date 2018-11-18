#bin/bash

perNum=` wc -l "$1"`
resNum=` wc -l "$2"`
resNum2=` wc -l "$3"`
for x in $perNum; do perNum=$x; break; done
for x in $resNum; do resNum=$x; break; done
for x in $resNum2; do resNum2=$x; break; done

./tongji/tongji.out $perNum $resNum $resNum2 $1 $2 $3
