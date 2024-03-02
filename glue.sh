#!/bin/bash

usage="bash glue.sh"

if [ -d "./ResizedImages" ] 
then
rmdir ./ResizedImages
fi

if [ -d "./Stripes" ] 
then
rmdir ./Stripes
fi

mkdir ResizedImages

if [ "$#" -lt "0" ] ;then
	echo "$usage"

else

	for i in *.bmp
	do 
 		./resize  $i 
  	done
fi 	
 
  for i in *Resized.bmp
  do
      mv $i ./ResizedImages
	done
mkdir Stripes
	for i in ResizedImages/*Resized.bmp
	do 
 		./extractstrip -85 $i 
  	done
 for i in ResizedImages/*_S*.bmp
do
mv $i ./Stripes
done

cp glue.c ./Stripes/
cd Stripes/
gcc glue.c -o glue
./glue *.bmp
