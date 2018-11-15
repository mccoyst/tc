#!/bin/bash

TCDIR=../phase3

g++ -c -std=c++98 -Wall $TCDIR/RTS.cxx

echo "	NORMAL COMPILATION"
for f in *.t
do
	echo "===== $f ====="
	$TCDIR/tc < $f > $f.s
	if [ -s "$f.s" ]
	then
		g++ RTS.o $f.s
		./a.out
		echo "status: $?"
	fi
	rm -f $f.s a.out
done

rm -f RTS.o
