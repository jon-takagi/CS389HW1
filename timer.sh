#!/bin/bash
for i in $(seq 12 26)
do
    vector_length=$((2**($i)))
    ./bin.out $vector_length 10
done


#https://stackoverflow.com/questions/3888754/how-to-do-exponentiation-in-bash
