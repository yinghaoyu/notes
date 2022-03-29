# 每天学一点GDB调试技巧

- starti 从第一条指令开始运行
- layout asm 快速查看汇编
- si(single step) 向前执行一条指令，后续按回车键会重复
- backtrace 打印调用栈
- info register 快速查看寄存器
- info inferiors 显示所有进程
- !ls 执行终端的ls命令
- !pmap processid 打印进程地址空间有什么(等价于cat /proc/[pid]/maps)
- p $rax 打印rax寄存器的内容
- watch $rax 监视rax的内容
- gdb的time travel debuging（有些指令不支持）
- target record-full 开始记录状态
- record stop 结束记录
- rsi gdb的记录了程序状态的快照，回退执行一条指令
