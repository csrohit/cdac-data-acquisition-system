obj-m += usb_rohit.o
usb_rohit-objs += ./src/usb_custom.o ./src/proc.o ./src/device.o 

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

insert:
	sudo insmod usb_rohit.ko

remove:
	sudo rmmod usb_rohit.ko

clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` clean

.PHONY: all clean

