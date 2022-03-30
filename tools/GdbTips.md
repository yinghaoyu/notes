# 每天学一点GDB调试技巧

## 基本命令

- info frame 查看当前函数的程序语言
- info source 显示当前的调试源文件
- starti 从第一条指令开始运行
- layout asm 快速查看汇编
- si(single step) 向前执行一条指令，后续按回车键会重复
- backtrace 打印调用栈
- info register 快速查看寄存器
- !ls 执行终端的ls命令
- p $rax 打印rax寄存器的内容
- watch $rax 监视rax的内容
- gdb的time travel debuging（有些指令不支持）
- target record-full 开始记录状态
- record stop 结束记录
- rsi gdb的记录了程序状态的快照，回退执行一条指令
- save breakpoints gdb.cfg 保存断点到文件
- source gdb.cfg 就可以直接加载断点了
- checkpoint 为当前程序状态设置一个快照
- info checkpoints 查看所有快照
- restart [checkpoint id] 恢复保存[checkpoint id]时的状态
- target remote localhost:1234 远端调试
- kill 终止正在调试的程序

## 进程调试

- info inferiors 显示所有进程
- !pmap processid 打印进程地址空间有什么(等价于cat /proc/[pid]/maps)
- show follow-fork-mode
- show detach-on-fork
- set follow-fork-mode child 当 mode 为 parent 时，程序在调用 fork 后**调试父进程，子进程不会受到影响**。当 mode 为 child 时，程序在调用 fork 后**调试子进程，父进程不会受到影响**
- set detach-on-fork off 当 mode 为 on 时，表示程序**只调试一个进程**（可以是父进程、子进程）。当 mode 为 off 时，父子进程都在gdb的控制之下，其中**一个进程正常的调试，另一个会被设置为暂停状态**
- inferior [num] 切换到[num]对应的进程
- catch fork 设置捕获点中断在fork事件上
