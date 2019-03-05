#!/bin/bash



for ((i=0 ; 10 - $i ; i++))
do
    ./bench_tcrypt.sh 1 200 'ecb(aes)' >> ret_ecb_aes.txt

done

for ((i=0 ; 10 - $i ; i++))
do
    ./bench_tcrypt.sh 1 500 'ecb(aes)' >> ret_ecb_aes_ac.txt
done

