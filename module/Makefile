<<<<<<< HEAD
# my optional module name
MODULE=trigger

# this two variables, depends where you have you raspberry kernel source and tools installed

CCPREFIX=/winston/raspberry/tools-master/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-
KERNEL_SRC= /usr/src/linux-rpi-3.12.y 


obj-m += ${MODULE}.o

module_upload=${MODULE}.ko

all: clean compile

compile:
	make ARCH=arm CROSS_COMPILE=${CCPREFIX} -C ${KERNEL_SRC} M=$(PWD) modules

clean:
	make -C ${KERNEL_SRC} M=$(PWD) clean


# this just copies a file to raspberry
install:
	scp ${module_upload} pi@raspberry:test/

info:
	modinfo  ${module_upload}
=======
# my optional module name
MODULE=trigger

# this two variables, depends where you have you raspberry kernel source and tools installed

CCPREFIX=/winston/raspberry/tools-master/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-
KERNEL_SRC=/winston/raspberry/linux-rpi-3.12.y


obj-m += ${MODULE}.o

module_upload=${MODULE}.ko

all: clean compile

compile:
	make ARCH=arm CROSS_COMPILE=${CCPREFIX} -C ${KERNEL_SRC} M=$(PWD) modules

clean:
	make -C ${KERNEL_SRC} M=$(PWD) clean


# this just copies a file to raspberry
install:
	scp ${module_upload} pi@raspberry:test/

info:
	modinfo  ${module_upload}
>>>>>>> 2b47014682dc918264a0671eac692b082b1bf03d
