#! /bin/bash

# adapt it to your needs
HDA="-drive file=myImage.img,format=raw"

FLAGS="--enable-kvm "

exec qemu-system-x86_64 ${FLAGS} \
     ${HDA}  \
     -net user -net nic \
     -boot c -m 2G
