#!/bin/bash
mkdir -p tcrypt/{mb_cbc,ak_ecb,sk_cbc}

modprobe aes-cbc-mb
for ((i=0 ; 5 - $i ; i++))
do
    ./bench_tcrypt.sh 1 600 'cbc(aes)' >> tcrypt/mb_cbc/bench_tcrypt_mb_cbc_aes
done
    ./process_data.sh tcrypt/mb_cbc/bench_tcrypt_mb_cbc_aes

rmmod aes-cbc-mb 

for ((i=0 ; 5 - $i ; i++))
do
    ./bench_tcrypt.sh 1 500 'ecb(aes)' >> tcrypt/ak_ecb/bench_tcrypt_ak_ecb_aes
done
    ./process_data.sh tcrypt/ak_ecb/bench_tcrypt_ak_ecb_aes

for ((i=0 ; 5 - $i ; i++))
do
    ./bench_tcrypt.sh 1 200 'cbc(aes)' >> tcrypt/sk_cbc/bench_tcrypt_sk_cbc_aes
done
    ./process_data.sh tcrypt/sk_cbc/bench_tcrypt_sk_cbc_aes


make -C ../src
mkdir -p libkcapi/{sk_cbc,ak_ecb}

for ((i=0; 5 - $i ; i++))
do
	../src/bench_sk_cbc_aes >> libkcapi/sk_cbc/bench_libkcapi_sk_cbc_aes
done
    ./process_data.sh   libkcapi/sk_cbc/bench_libkcapi_sk_cbc_aes

for ((i=0; 5 - $i ; i++))
do 
	../src/bench_ak_ecb_aes >> libkcapi/ak_ecb/bench_libkcapi_ak_ecb_aes
done
    ./process_data.sh libkcapi/ak_ecb/bench_libkcapi_ak_ecb_aes

make -C ../src clean