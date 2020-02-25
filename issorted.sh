#!/bin/bash

name=sort 

gcc -Wall -Wextra -O2 -o $name sort.c

for f in *.in
do
  ./$name <$f >${f%in}out
  echo "Dla pliku $f program zakończył się kodem $?."
done

for f in *.in
do 
	echo -n "Plik $f "
	if diff ${f%in}out <(sort -g $f) >/dev/null 2>&1
	then 
		echo "został posortowany."
	else 
		echo "nie został posortowany."
	fi
done	