#! /bin/bash

# disk hard drive, adapt it to your needs
HDA="-drive file=linux.img,format=raw"

KERNEL="kernel/vmlinuz-4.11-rc3-intel"

# shared direcroty with the guest 
SHARED=./share/

# Linux kernel options 

# x to be substitued with the / partition number
CMDLINE="root=/dev/sdaX rw console=ttyS0 kgdboc=ttyS1 "

FLAGS="--enable-kvm "
VIRTFS+=" --virtfs local,path=${SHARED},mount_tag=share,security_model=passthrough,id=share "

exec qemu-system-x86_64 ${FLAGS} \
     ${HDA} \
     ${VIRTFS} \
     -net user -net nic \
     -serial mon:stdio \
     -serial tcp::1234,server,nowait \
     -boot c \
     -m 2G \
     -kernel "${KERNEL}" \
     -append "${CMDLINE}"
