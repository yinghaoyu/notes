### Tcpdump

```
route -n // 显示路由表

arp -a  // 显示arp路由

arp -d ip地址 // 删除arp表的该ip地址对应的项

arp -d ip地址 && curl www.baidu.com //  因为此时删除了arp，可以用tcpdump捕捉arp协议包

tcpdump -nn -X port 80 -i eth0 or arp  // -X表示显示捕捉详细内容
```

### strace
```
strace -ff -o out ./test // 抓取test所有线程的系统调用并输出到out.tid文件
strace -f gcc hello.c 2>&1 | grep execve // 2>&1的意思就是将标准错误重定向到标准输出，这里标准输出已经重定向到了 /dev/null
```

### ltrace
```
ltrace ls // 捕捉程序调用的动态库函数
```
LD_PRELOAD，是个环境变量，用于动态库的加载，动态库加载的优先级最高
### File
```
cat /etc/services | grep telnet // 查看服务的端口号
```

### sudo
```
sudo !! // 会在命令行自动用sudo补充上一条命令
```

### date
```
date -d @12312343452 // 打印时间戳的UTC
```

### 设置进程的nice值（优先级）
nice的值介于-20~19直接，值越小分配的cpu占比越大
```
renice -n [value] [pid]
```

### 绑定进程到cpu上
taskset -cp [cpuid] [pid]
