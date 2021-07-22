很多人以为在main函数中return和调用exit差不多，反正都是程序结束了。但其实并不是这样的。

## fork与vfork

想一想下面两种情况会发生什么？

- 分别在fork出的子进程中调用return和exit。

- 分别在vfork出的子进程中调用return和exit。

在fork出的子进程的main函数中调用return和exit，都无异常情况发生，程序正常运行退出。在vfork出的子进程中调用exit，程序正常退出，而在vfork出的子进程的main函数中return，程序就直接挂掉了。

先说说fork与vfork的区别：

- fork是创建一个子进程并得到与父进程相同的虚拟地址空间的一份独立的拷贝，也就是把父进程的内存数据都拷贝到子进程中了。（在实际中，一般采用的是写时复制Copy-On-Write）

- vfork是创建一个子进程并与父进程共享虚拟地址空间（父进程的），也就是和父进程共享所有内存数据。

那么为什么会有vfork这么一个玩意呢？据说一开始unix下只有fork，但是很多程序员喜欢在fork后立马使用exec执行一个外部程序，于是fork需要复制父进程的内存数据这一动作就变得毫无意义了（注：开始的时候fork并没有使用COW机制）。于是就弄出了与父进程共享数据的vfork，方便高效的使用exec执行外部程序。

再回到上面的问题，为什么在vfork出的子进程的main函数里return程序就直接crash掉了？

其实这个过程是这样的：

1. 子进程中的main函数ruturn。

2. main函数return后会调用exit函数。

3. 父进程发现自己的栈因子进程return而被弹掉了，还怎么让我执行，于是就直接crash掉了。

所以，可见main函数中return会先弹栈然后再调用exit，而直接调用exit并不会修改栈。

## 析构函数

析构函数与return、exit有什么关系呢？看下面一个例子：

```cpp
class Test
{
public:
  Test()
  {
    printf("start\n");
  }
  ~Test()
  {
    printf("end\n");
  }
};

int main(int argc, char* argv[])
{
  Test test;
  //exit(0);
  return 0;
}
```

分别使用return和exit运行程序会发现，只有运行return的时候才出现end打印，而执行exit的时候并不会打印end，也就是没有执行析构函数。

但是，当我们把test变成全局变量或者local static变量时，情况又不一样了：

```cpp
Test test;

int main(int argc, char* argv[])
{
    //static Test test;
    exit(0);
    //return 0;
}
```

此时，无论是使用全局变量还是local static变量，无论是执行exit函数还是return都会出现「end」打印。

可见，exit退出时不会析构局部变量，但是会析构全局变量和local static变量，而main函数return时是先析构局部变量，然后调用exit（自然也就析构了全局变量和local static变量了，按照《深度探索C++对象模型》中的说法，其中一种实现方法是将执行析构所有全局变量操作的函数链接到标准库函数exit上）。

## exit与_exit

说到exit就不得不提_exit了，还是沿用上面的例子：

```cpp
//Test test

int main(int argc, char* argv[])
{
    Test test;
    //static Test test;
    _exit(0);
}
```

我们发现对于变量test，无论它是局部变量、全局变量还是local static变量，都不会出现end打印。即无论何时_exit都不会析构任何变量。

Linux man page中关于_exit是这样描述的：

>The function _exit() terminates the calling process “immediately”. Any open file descriptors belonging to the process are closed; any children of the process are inherited by process 1, init, and the process’s parent is sent a SIGCHLD signal.
The function _exit() is like exit(3), but does not call any functions registered with atexit(3) or on_exit(3). Whether it flushes standard I/O buffers and removes temporary files created with tmpfile(3) is implementation-dependent. On the other hand, _exit() does close open file descriptors, and this may cause an unknown delay, waiting for pending output to finish. If the delay is undesired, it may be useful to call functions like tcflush(3) before calling _exit(). Whether any pending I/O is canceled, and which pending I/O may be canceled upon _exit(), is implementation-dependent.

其实在调用_exit时，它会关闭已打开的文件描述符（不刷新IO缓冲区），并调用sys_exit系统调用退出程序。

而在调用exit时，它会刷新清理IO缓冲区（包括文件缓冲区，标准输入输出缓冲区），检查文件打开状态，关闭已打开的文件描述符，执行所有通过atexit注册的函数（按与注册时相反的顺序调用）。

## 总结

- 调用exit时会析构全局变量和local static变量（不析构局部变量），刷新IO缓冲区，关闭文件描述符，调用atexit注册的函数（以注册时相反的顺序调用），但不弹栈。

- 无论何时调用return，都会析构局部变量，并弹栈。在main函数中调用return，会析构局部变量、弹栈，然后执行exit（包括了析构全局变量和local static变量的过程）。

- 直接调用_exit不会析构任何变量，不会刷新IO缓冲区，不会执行任何通过atexit注册的函数，不弹栈，但会关闭文件描述符。

## 参考文献
1. Stanley B. Lippman. [深度探索C++对象模型. 电子工业出版社, 2012](https://book.douban.com/subject/10427315/)
