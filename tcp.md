![tcp1](/images/tcp1.png)

MSL:Maximum Segment Lifetime

1. TCP总共有几种状态？

> 11种。分别为CLOSED, SYN_SEND, SYN_RECVD, ESTABLISH, LISTEN, FIN_WAIT-1, CLOSE_WAIT, FIN_WAIT_2, LAST_ACK, TIME_WAIT, CLOSING。其中CLOSING状态是A、B两端同时发起close时，产生的中间状态。

2. 如何解决过多的TIME_WAIT状态？

> TIME_WAIT一般是主动发起close方的状态，可以对socket设置SO_REUSEADDR选项。

3. B端进程为什么会触发SIG_PIPE信号？以及如何解决？

> “对一个已经收到FIN包的socket调用read方法, 如果接收缓冲已空, 则返回0, 这就是常说的表示连接关闭。
但第一次对其调用write方法时, 如果发送缓冲没问题, 会返回正确写入(发送)。但发送的报文会导致对端发送RST报文, 因为对端的socket已经调用了close, **完全关闭, 既不发送, 也不接收数据**。
所以, 第二次调用write方法(假设在收到RST之后), 会生成SIGPIPE信号, 导致进程退出。”可以使用SIGNAL(SIG_PIPE, SIG_IGN)忽略，或者注册一个handler函数处理。

4. 什么时候会发送FIN包？

> close后，socket**引用计数为0**，会发送FIN包，否则不会发送。因为有可能子进程继承了父进程的fd，没有对socket设置SOCK_CLOEXEC选项。

5. showdown和close有什么区别？

> close不管输出缓冲区中是否还有数据，而shutdown会等输出缓冲区中的数据传输完毕再发送FIN包。也就意味着，调用close将丢失输出缓冲区中的数据，而调用shutdown不会。shotdown可以单向关闭写或读（half-close）。

本博客所有原创文章均采用[CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)许可协议。

您可以复制共享、演绎创作，但不得用于商业目的，转载请注明原创作者 **raining888** 。
