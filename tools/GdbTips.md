# 每天学一点GDB调试技巧

## 基本命令

- 如何查看当前函数的程序语言？`info frame`
- 如何显示当前的调试源文件？`info source`
- 如何从第一条指令开始运行？`starti`
- 如何从main第一条指令开始运行？`start`
- 如何快速查看汇编？`layout asm` 
- 如何快速查看源码？`layout src` 
- 如何向前执行一条指令？`si(single step)`
- 如何重复执行一条指令？`回车`
- 如何查看调用堆栈？`bt(backtrace)` 
- 如何查看寄存器信息？`info register` 
- 如何执行终端的ls命令？`!ls` 
- 如何打印rax寄存器的内容？`p $rax` 
- 如何监视rax的内容？`watch $rax` 
  > watch 变化时，停住程序。 rwatch 被读时，停住程序。 awatch 被读或者被写，停住程序。
- 如何列出所有监视？`info watchpoints`
- 哪些指令不支持time travel debuging？`syscall等`
- 如何开始记录程序状态？`target record-full` 
- 如何结束记录程序状态？`record stop` 
- 如何回退执行一条指令？(前提记录了程序状态的快照)`rsi` 
- 如何保存断点到文件？`save breakpoints gdb.cfg`
- 如何加载断点文件？`source gdb.cfg`
- 如何为当前程序状态设置一个快照？`checkpoint` 
- 如何查看所有快照？`info checkpoints` 
- 如何恢复快照？`restart [checkpoint id]`
- 如何远端调试？`target remote localhost:1234`
- 如何终止正在调试的程序？`kill`
- 如何打印变量的值？`print /x var 其中/x表示以16进制打印var变量`
- 如何打印变量的地址？`print &var`
- 如何打印地址的数据值？`print *address`
- 如何立即执行完当前的函数，但是并不是执行完整个应用程序？`finish`
- 如何查看指定文件的代码？`list file:N`
- 如果循环次数很多，如何执行完当前的循环？`until`
- 如何在程序启动后调试？`gdb [pname] [pid]`
- 如何调试core文件？`gdb [pname] [core file]`
- 如何设置启动参数？`set args 10 20 30`
- 如何设置程序运行的路径？`path [dir]`
- 如何查看程序运行的路径？`show paths`
- 如何设置程序的环境变量？`set env USER=hchen`
- 如何查看程序的环境变量？`show environment [var]`
## 进程调试

- 如何显示所有进程？`info inferiors`
- 如何查看进程地址空间？`!pmap processid (等价于cat /proc/[pid]/maps)`
- 如何查看默认调试的是父进程还是子进程？`show follow-fork-mode`
- 如何查看默认调试的是单进程还是多进程？`show detach-on-fork`
  > set follow-fork-mode child 当 mode 为 parent 时，程序在调用 fork 后**调试父进程，子进程不会受到影响**。当 mode 为 child 时，程序在调用 fork 后**调试子进程，父进程不会受到影响**

  > set detach-on-fork off 当 mode 为 on 时，表示程序**只调试一个进程**（可以是父进程、子进程）。当 mode 为 off 时，父子进程都在gdb的控制之下，其中**一个进程正常的调试，另一个会被设置为暂停状态**
- 如何切换调试的进程？`inferior [num]`
- 如何捕获fork事件？`catch fork`
