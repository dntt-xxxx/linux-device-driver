obj-m := main.o

#KDIR = /lib/modules/4.19.94-ti-r42/build

KDIR = /home/tudo/workdir/tmp/lib/modules/4.19.94/build

all:
	make -C $(KDIR)  M=$(shell pwd) modules
clean:
	make -C $(KDIR)  M=$(shell pwd) clean


