#!/bin/bash
if [ $# -ne  3 ];
then
        echo "error: use bench_tcrypt.sh sec mode ciphername"
else
	dmesg -C
	modprobe tcrypt mode=$2 sec=$1 
	dmesg | sed -n '/\s'$3'\s.*encryption/,/testing/p'
fi
