### Tcpdump

```bash
route -n // 显示路由表

arp -a  // 显示arp路由

arp -d ip地址 // 删除arp表的该ip地址对应的项

arp -d ip地址 && curl www.baidu.com //  因为此时删除了arp，可以用tcpdump捕捉arp协议包

tcpdump -nn -X port 80 -i eth0 or arp  // -X表示显示捕捉详细内容
```

### strace
```bash
strace -ff -o out ./test // 抓取test所有线程的系统调用并输出到out.tid文件
strace -f gcc hello.c 2>&1 | grep execve // 2>&1的意思就是将标准错误重定向到标准输出，这里标准输出已经重定向到了 /dev/null
```

### ltrace
```bash
ltrace ls // 捕捉程序调用的动态库函数
```
LD_PRELOAD，是个环境变量，用于动态库的加载，**动态库加载的优先级最高**
### File
```bash
cat /etc/services | grep telnet // 查看服务的端口号
```

### sudo
```bash
sudo !! // 会在命令行自动用sudo补充上一条命令
```

### date
```bash
date -d @12312343452 // 打印时间戳的UTC
```

### 设置进程的nice值（优先级）
nice的值介于-20~19直接，值越小分配的cpu占比越大
```bash
renice -n [value] [pid]
```

### 绑定进程到cpu上
```bash
taskset -cp [cpuid] [pid]

taskset -c 0 nice -n 9 yes > /dev/null &
taskset -c 0 nice -n 19 yes > /dev/null &
top // 可以查看两个进程的cpu占有率
```
### perf
```bash
perf record --call-graph dwarf timeout 1 ./a.out //收集执行1s的a.out的perf.data
perf report // 查看统计的数据，主要是系统调用花费的时间等等
```
### tty
```bash
stty -a // 可以查看终端的信息
```

###  查看总线
```bash
lspci -tv  //pci
lsusb -tv  //usb
```

### sort
```bash
sort -nk 6 // 把第6列当数字比较排序
```

### timeout
```bash
timeout 1 ./a.out // 就算a.out是死循环，1秒后结束a.out的运行
```

### 如何让a.out重复运行(不修改a.out代码)?
```bash
while true; do ./a.out; done
```

### 统计输出
```bash
./a.out | head -n 10000 | sort | uniq -C // 取出前10000行，排序，统计
```

### 查看系统内存
```bash
free -m
free -h
```

### 查看Makefile的所有编译
```bash
make -nB
```

### Bear 捕捉compile_commands.json
```bash
bear make
```

### 挂载文件系统
```bash
mount /usr/sda /mnt // 把/usr/sda挂载到/mnt上
umount /mnt // 卸载文件系统
lsblk // 查看块设备
```
