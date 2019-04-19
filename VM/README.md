This direcroty contains the elements needed to start a KVM/QEMU virtual machine. 

*kernel/* : contains a Linux kernel image in which the [intel](https://github.com/meghadey/crypto) AES multi-buffer is implemented. 

*share/* : a shared directory bewteen the guest and the host, contains an archive of modules such as the testing module `tcrypt` which we'll be needing during the developpement of this project. To extract the modules archive under */lib/modules/* the following command should be typed within the *share/* directory:  `tar -C /lib/modules/ -zxvf modules.tar.gz`. 

NOTE: if `modprobe` can't find the tcrypt module under */lib/modules*, just run `depmod` to generate an up-to-date list of module dependencies which `modprobe` needs. 


*qemu-run.sh* : script to launch the QEMU/KVM virtual machine, adapt it to your needs before executing it. 

*qemu-run-externKernel.sh* : script to launch the KVM/QEMU virtual machine with external kernel, adapt it to your needs before executing it. You might want to adapt the *qemu-run.sh* script first. 
