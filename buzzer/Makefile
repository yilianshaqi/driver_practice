ifeq ($(KERNELRELEASE),)
KERNELDIR ?= /home/hy/linux-3.14
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
else
    obj-m :=buzzer.o
endif

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module* modules*
