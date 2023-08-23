#!/bin/bash

g++ -Wall -pedantic -O2 Hexadoku.c -o Hexadoku

PROG=./Hexadoku

for IN_FILE in data/cze/*_in.txt ; do
	REF_FILE=`echo -n $IN_FILE | sed "s/_in/_out/"`
	$PROG < $IN_FILE > my_out.txt
	if ! diff $REF_FILE my_out.txt ; then
		echo "Fail: $IN_FILE\n";
		echo "REF FILE: $REF_FILE";
		exit
	else
		echo "OK: $IN_FILE";
	fi
done

for IN_FILE in data/extra/cze/*_in.txt ; do
	REF_FILE=`echo -n $IN_FILE | sed "s/_in/_out/"`
	$PROG < $IN_FILE > my_out.txt
	if ! diff $REF_FILE my_out.txt ; then
		echo "Fail: $IN_FILE\n";
		echo "REF FILE: $REF_FILE";
		exit
	else
		echo "OK: $IN_FILE";
	fi
done




