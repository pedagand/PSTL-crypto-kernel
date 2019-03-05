#!/bin/bash
#args SEC MODE CIPHERNAME

sudo dmesg -C
sudo modprobe tcrypt mode=$2 sec=$1
dmesg | sed -n '/'$3'/,/testing/p'
