对于程序来说，如果主进程在子进程还未结束时就已经退出(子进程变为孤儿进程)，那么Linux内核会将子进程的父进程ID改为1（init进程收养孤儿进程），当子进程结束后会由init进程来回收该子进程。

那如果是把进程换成线程的话，会怎么样呢？假设主线程在子线程结束前就已经退出，子线程会发生什么？

在一些论坛上看到许多人说子线程也会跟着退出，其实这是错误的，原因在于他们混淆了线程退出和进程退出概念。实际的答案是主线程退出后子线程的状态依赖于它所在的进程，如果进程没有退出的话子线程依然正常运转。如果进程退出了，那么它所有的线程都会退出，所以子线程也就退出了。

## 主线程先退出
先来看一个主线程先退出的例子：
```cpp
#include <pthread.h>
#include <unistd.h>

#include <stdio.h>

void* func(void* arg)
{
    pthread_t main_tid = *static_cast<pthread_t*>(arg);
    pthread_cancel(main_tid);
    while (true)
    {
        //printf("child loops\n");
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t main_tid = pthread_self();
    pthread_t tid = 0;
    pthread_create(&tid, NULL, func, &main_tid);
    while (true)
    {
        printf("main loops\n");
    }
    sleep(1);
    printf("main exit\n");
    return 0;
}
```
把主线程的线程号传给子线程，在子线程中通过pthread_cancel终止主线程使其退出。运行程序，可以发现在打印了一定数量的「main loops」之后程序就挂起了，但却没有退出。
主线程因为被子线程终止了，所有没有看到「main exit」的打印。子线程终止了主线程后进入了死循环while中，所以程序看起来像挂起了。如果我们让子进程while循环中的打印语句生效再运行就可以发现程序会一直打印「child loops」字样。

主线程被子线程终止了，但他们所依赖的进程并没有退出，所以子线程依然正常运转。

## 主线程随进程一起退出
之前看到一些人说如果主线程先退出了，子线程也会跟着退出，其实他们混淆了线程退出和进程退出的概念。下面这个例子代表了他们的观点：
```cpp
void* func(void* arg)
{
    while (true)
    {
        printf("child loops\n");
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t main_tid = pthread_self();
    pthread_t tid = 0;
    pthread_create(&tid, NULL, func, &main_tid);
    sleep(1);
    printf("main exit\n");
    return 0;
}
```
运行上面的代码，会发现程序在打印一定数量的「child loops」和一句「main exit」之后退出，并且在退出之前的最后一句打印是「main exit」。
按照他们的逻辑，你看，因为主线程在打印完「main exit」后退出了，然后子线程也跟着退出了，所以随后就没有子线程的打印了。

但其实这里是混淆了进程退出和线程退出的概念了。实际的情况是主线程中的main函数执行完return后弹栈，然后调用glibc库函数exit，exit进行相关清理工作后调用_exit系统调用退出该进程。所以，这种情况实际上是因为进程运行完毕退出导致所有的线程也都跟着退出了，并非是因为主线程的退出导致子线程也退出。

## Linux线程模型
实际上，posix线程和一般的进程不同，在概念上没有主线程和子线程之分（虽然在实际实现上还是有一些区分），如果仔细观察APUE或者UNP等书会发现基本看不到「主线程」或者「子线程」等词语，在CSAPP中甚至都是用「对等线程」一词来描述线程间的关系。

在Linux 2.6以后的posix线程都是由用户态的pthread库来实现的。在使用pthread库以后，在用户视角看来，每一个tast_struct就对应一个线程（tast_struct原本是内核对应一个进程的结构），而一组线程以及他们所共同引用的一组资源就是进程。从Linux 2.6开始，内核有了线程组的概念，tast_struct结构中增加了一个tgid（thread group id）字段。getpid（获取进程号）通过系统调用返回的也是tast_struct中的tgid，所以tgid其实就是进程号。而tast_struct中的线程号pid字段则由系统调用syscall(SYS_gettid)来获取。

当线程收到一个kill致命信号时，内核会将处理动作施加到整个线程组上。为了应付「发送给进程的信号」和「发送给线程的信号」，tast_struct里面维护了两套signal_pending，一套是线程组共用的，一套是线程独有的。通过kill发送的致命信号被放在线程组共享的signal_pending中，可以任意由一个线程来处理。而通过pthread_kill发送的信号被放在线程独有的signal_pending中，只能由本线程来处理。

关于线程与信号，APUE有这么几句：

> 每个线程都有自己的信号屏蔽字，但是信号的处理是进程中所有线程共享的。这意味着尽管单个线程可以阻止某些信号，但当线程修改了与某个信号相关的处理行为以后，所有的线程都必须共享这个处理行为的改变。这样如果一个线程选择忽略某个信号，而其他的线程可以恢复信号的默认处理行为，或者是为信号设置一个新的处理程序，从而可以撤销上述线程的信号选择。
**如果信号的默认处理动作是终止该进程，那么把信号传递给某个线程仍然会杀掉整个进程。**

例如一个程序a.out创建了一个子线程，假设主线程的线程号为9601，子线程的线程号为9602（它们的tgid都是9601），因为默认没有设置信号处理程序，所以如果运行命令kill 9602的话，是可以把9601和9602这个两个线程一起杀死的。如果不知道Linux线程背后的故事，可能就会觉得遇到灵异事件了。

另外系统调用syscall(SYS_gettid)获取的线程号与pthread_self获取的线程号是不同的，pthread_self获取的线程号仅仅在线程所依赖的进程内部唯一，在pthread_self的man page中有这样一段话：

> Thread IDs are guaranteed to be unique only within a process. A thread ID may be reused after a terminated thread has been joined, or a detached thread has terminated.

所以在内核中唯一标识线程ID的线程号只能通过系统调用syscall(SYS_gettid)获取。

## 参考文献
1. W.Richard Stevens. [UNIX环境高级编程(第3版), 人民邮电出版社, 2014](https://book.douban.com/subject/25900403/)
