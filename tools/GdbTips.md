# 保姆级GDB调试入门

## 基本命令

- 如何查看当前函数的程序语言？`info frame`
- 如何显示当前的调试源文件？`info source`
- 如何从第一条指令开始运行？`starti`
- 如何从main第一条指令开始运行？`start`
- 如何查看汇编？`disassemble`
  > disassemble [function] 查看函数汇编，disassemble /r同时显示原始机器码
- 如何修改汇编格式？`set disassembly-flavor intel`
  > set disassembly-flavor只能用在Intel x86处理器上，并且取值只有intel和att。
- 如何查看汇编和源代码的映射？`disassemble /m [function]`
- 如何分窗口查看汇编？`layout asm` 
- 如何分窗口查看源码？`layout src` 
- 如何关闭分窗口？`Ctrl X A` 
- 如何向前执行一条指令？`si(single step)`
- 如何重复执行一条指令？`回车`
- 如何跳转到某一行执行？`jump [linenumber]`
- 如何显示即将要执行的一条指令？`display /i $pc`
  > display /3i $pc 显示即将要执行的3条指令
- 如何查看所有函数？`info functions`
  > 可以使用正则表达式，info functions thre* 返回thre开头的所有函数
- 如何强制返回当前函数？`return`
- 如何查看调用堆栈？`bt(backtrace)` 
- 如何查看寄存器信息？`info registers`
  > info all-registers 输出所有寄存器包括浮点寄存器和向量寄存器。
- 如何执行终端的ls命令？`!ls` 
- 如何打印rax寄存器的内容？`p $rax` 
- 如何监视rax的内容？`watch $rax` 
  > watch 变化时，停住程序。 rwatch 被读时，停住程序。 awatch 被读或者被写，停住程序。
- 如何列出所有监视？`info watchpoints`
  > 监视也支持disable、enable、delete操作
- 哪些指令不支持time travel debuging？`syscall等`
- 如何开始记录程序状态？`target record-full` `record full`**不是万能的有些复杂的syscall不能记录**
- 如何结束记录程序状态？`record stop` 
- 如何回退执行一条指令？(前提记录了程序状态的快照)`rsi` 
- 如何添加条件断点？`breakpoint file:N if [var] > 0`
- 如何忽略count次断点？`ignore bnum count`
- 如何保存断点到文件？`save breakpoints gdb.cfg`
- 如何加载断点文件？`source gdb.cfg`
- 如何为当前程序状态设置一个快照？`checkpoint` 
- 如何查看所有快照？`info checkpoints` 
- 如何恢复快照？`restart [checkpoint id]`
- 如何远端调试？`target remote localhost:1234`
- 如何终止正在调试的程序？`kill`
- 如何打印变量的值？`print /x var 其中/x表示以16进制打印var变量`
  > 可以支持的变量显示格式有:
  > 
  > - x 按十六进制格式显示变量。
  > - d 按十进制格式显示变量。
  > - u 按十六进制格式显示无符号整型。
  > - o 按八进制格式显示变量。
  > - t 按二进制格式显示变量。
  > - a 按十六进制格式显示变量。
  > - c 按字符格式显示变量。
  > - f 按浮点数格式显示变量。
- 如何打印内存值？`x/nfu addr`
  > - n：输出单元的个数。
  > - f：是输出格式。比如x是以16进制形式输出，o是以8进制形式输出,等等。
  > - u：标明一个单元的长度。b是一个byte，h是两个byte（halfword），w是四个byte（word），g是八个byte（giant word）。
- 如何打印数组的指定元素个数？`print *arr@10`
- 如何打印从下标i开始指定j个元素的数组？`print arr[i]@[j]`
- 如何打印数组的时候同时打印索引下标？`set print array-indexes on`
- 如何设置无限制打印数组所有元素？`set print elements 0`
- 如何打印变量的地址？`print &var`
- 如何打印地址的数据值？`print *address`
- 如何打印所有函数的局部变量？`bt full`
- 如何打印当前函数的局部变量？`info locals`
- **如何打印STL容器中的内容？**
- 如何按照派生类型打印类对象？` set print object on`
- **如何打印程序动态分配内存的信息？**
- 如何显示变量的类型？`whatis var`
  > ptype var 以更详细的方式显示变量var的类型。
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
- 如何查看信号的处理？`info signals或info handle`
- 如何不处理信号？`handle SIGHUP nopass`
- 如何恢复信号处理？` handle SIGHUP pass`
- 如何给进程发送信号？`signal SIGHUP`
- 如何列出所有共享库？`info sharedlibrary`
  > info sharedlibrary regex 可以附带正则表达式去查询某个共享库。
- 如何设置查找的源文件路径？`directory [dir]`
- 如何替换源文件的查找路径？`substitute-path [new dir] [old dir]`
## 进程调试

- 如何显示所有进程？`info inferiors`
- 如何查看当前进程的内存映射？`info proc mappings`
  > info files 或 info target更详细地输出进程的内存信息，包括引用的动态链接库等等
- 如何查看进程地址空间？`!pmap processid (等价于cat /proc/[pid]/maps)`
- 如何查看默认调试的是父进程还是子进程？`show follow-fork-mode`
- 如何查看默认调试的是单进程还是多进程？`show detach-on-fork`
  - set follow-fork-mode child 当 mode 为 parent 时，程序在调用 fork 后**调试父进程，子进程不会受到影响**。当 mode 为 child 时，程序在调用 fork 后**调试子进程，父进程不会受到影响**
  - set detach-on-fork off 当 mode 为 on 时，表示程序**只调试一个进程**（可以是父进程、子进程）。当 mode 为 off 时，父子进程都在gdb的控制之下，其中**一个进程正常的调试，另一个会被设置为暂停状态**
- 如何切换调试的进程？`inferior [num]`
- 如何捕获事件？`catch <event>`
  > tcatch <event> 只设置一次捕捉点，当程序停住以后，该点被自动删除。
  >
  > 一般能捕获的事件有：
  > - throw 一个C++抛出的异常。（throw为关键字）
  > - catch 一个C++捕捉到的异常。（catch为关键字）
  > - exec 调用系统调用exec时。（exec为关键字，目前此功能只在HP-UX下有用）
  > - fork 调用系统调用fork时。（fork为关键字，目前此功能只在HP-UX下有用）
  > - vfork 调用系统调用vfork时。（vfork为关键字，目前此功能只在HP-UX下有用）
  > - syscall [function] 调用系统调用function时。
  > - load 或 load <libname> 载入共享库（动态链接库）时。（load为关键字，目前此功能只在HP-UX下有用）
  > - unload 或 unload <libname> 卸载共享库（动态链接库）时。（unload为关键字，目前此功能只在HP-UX下有用）
- 如何查看该变量可能定义的文件？`info variables var`
  > info variables "^var$" 更严格的筛选
- 如何调试已运行的进程？`attach [pid]`
- 如何放弃调试已运行的进程？`detach`
- 如何添加调试其他进程？`add-inferior [ -copies number ] [ -exec executable ]命令加载可执行文件b`
- 如何打印进程空间信息？`maint info program-spaces`

## 线程调试

- 如何只允许一个线程运行？`set scheduler-locking on`
  > set scheduler-locking 命令除了支持off和on模式外（默认是off），还有一个step模式。含义是：当用"step"命令调试线程时，其它线程不会执行，但是用其它命令（比如"next"）调试线程时，其它线程也许会执行.
- 如何查看所有线程？`info threads`
- 如何切换线程？`thread [tid]`
- 如何使监视只对某线程生效？`watch [val] thread [threadnum]`
  
## 避开ptrace反追踪
```c
#include <sys/ptrace.h>
#include <stdio.h>

int main()                                                                      
{
  if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0 )
  {
     printf("Gdb is debugging me, exit.\n");
     return 1;
  }
  printf("No debugger, continuing\n");
  return 0;
}
```
先通过捕获系统调用`catch syscall ptrace`，然后通过`set $eax = 0`强制修改ptrace的返回值即可

## GDB脚本
在`.gdbinit`文件中可以加入以下配置：
```
# 打印STL容器中的内容
python
import sys
sys.path.insert(0, "/home/xmj/project/gcc-trunk/libstdc++-v3/python")
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers (None)
end

# 保存历史命令
set history filename ~/.gdb_history
set history save on

# 退出时不显示提示信息
set confirm off

# 按照派生类型打印对象
set print object on

# 打印数组的索引下标
set print array-indexes on

# 每行打印一个结构体成员
set print pretty on
```
## 参考资料
[GDB: The GNU Project Debugger](https://sourceware.org/gdb/documentation/)
