#!/bin/bash

name=lasy

make

for f in tests/*.in
do
  h=${f%in}
  h2=${h##*/}
  ./$name <$f >tests/out/${h2}out 2>tests/out/${h2}err
  echo "Dla pliku $f program zakończył się kodem $?."
done

make clean

#for f in *.in
#do
#	echo -n "Plik $f "
#	if diff ${f%in}out <(sort -g $f) >/dev/null 2>&1
#	then
#		echo "został posortowany."
#	else
#		echo "nie został posortowany."
#	fi
#done