#!/bin/bash

PROG=./Hexadoku
TESTS_DIR=data/cze

# script to test the program and output the time it takes to solve the puzzle
# usage: ./testtime.sh <input file> 

# get the input files from the folder tests/
for IN_FILE in $TESTS_DIR/*_in.txt ; do
    # get the reference file from the folder tests/
    REF_FILE=`echo -n $IN_FILE | sed "s/_in/_out/"`
    # run the program and output the time it takes to solve the puzzle
    { time $PROG < $IN_FILE > my_out.txt ; } 2> time.txt
    # if the output is different from the reference file, print the error
    if ! diff $REF_FILE my_out.txt ; then
        echo "Fail: $IN_FILE\n";
        echo "REF FILE: $REF_FILE";
        exit
    else
        # if the output is the same as the reference file, print the time it took to solve the puzzle
        echo "OK: $IN_FILE" | tr '\n' ' '
        cat time.txt | tr '\n' ' ' | tr -s ' '  
        echo ""
    fi
done

echo ''

TESTS_DIR=data/extra/CZE
# get the input files from the folder tests/
for IN_FILE in $TESTS_DIR/*_in.txt ; do
    # get the reference file from the folder tests/
    REF_FILE=`echo -n $IN_FILE | sed "s/_in/_out/"`
    # run the program and output the time it takes to solve the puzzle
    { time $PROG < $IN_FILE > my_out.txt ; } 2> time.txt
    # if the output is different from the reference file, print the error
    if ! diff $REF_FILE my_out.txt ; then
        echo "Fail: $IN_FILE\n";
        echo "REF FILE: $REF_FILE";
        exit
    else
        # if the output is the same as the reference file, print the time it took to solve the puzzle
        echo "OK: $IN_FILE" | tr '\n' ' '
        cat time.txt | tr '\n' ' ' | tr -s ' '  
        echo ""
    fi
done