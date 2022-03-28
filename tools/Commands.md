### Tcpdump

```
route -n // 显示路由表

arp -a  // 显示arp路由

arp -d ip地址 // 删除arp表的该ip地址对应的项

arp -d ip地址 && curl www.baidu.com //  因为此时删除了arp，可以用tcpdump捕捉arp协议包

tcpdump -nn -X port 80 -i eth0 or arp  // -X表示显示捕捉详细内容
```

### Strace
```
strace -ff -o out ./test // 抓取test所有线程的系统调用并输出到out.tid文件
strace -f gcc hello.c 2>&1 | grep execve // 2>&1的意思就是将标准错误重定向到标准输出，这里标准输出已经重定向到了 /dev/null
```
