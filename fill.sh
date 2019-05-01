#!/bin/bash

x=-10
y=0
z=4

for (( c=0; c<100; c++ ))
do
	echo "" >> scenes/generated.txt
	echo "SPHERE" >> scenes/generated.txt
	echo "position: $x, $y, $z" >> scenes/generated.txt
	echo "color: 102, 153, 255" >> scenes/generated.txt
	(( x+=2 ))
	if [ $x -eq 10 ]
	then
		(( x=-6 ))
		(( y+=2))
	fi
done