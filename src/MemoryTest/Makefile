KVERS = $(shell uname -r)
#shell中输出uname -r会获得内核版本号，这里将版本号存到变量KVERS
# Kernel modules
# #将后面的东东编译为“内核模块”， obj-y 编译进内核，obj-n 不编译。
obj-m += kernelTest.o
#
# # 开启EXTRA_CFLAGS=-g-O0，可以得到包含调试信息的hello.ko模块。
# #EXTRA_CFLAGS=-g -O0
# #-C 表示让 make 命令进入指定的内核源代码目录
build: kernel_modules
kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules
clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
