#bin/bash

perNum=` wc -l "$1"`
resNum=` wc -l "$2"`
for x in $perNum; do perNum=$x; break; done
for x in $resNum; do resNum=$x; break; done

./tongji/tongji.out $perNum $resNum $1 $2
