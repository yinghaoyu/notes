![tcp](/images/tcp.png)

MSL:Maximum Segment Lifetime

- 如何解决过多的TIME_WAIT状态？

> 设置SO_REUSEADDR选项。

- B端进程为什么会触发SIG_PIPE信号？以及如何解决？

> 在CLOSE_WAIT状态时，TCP属于单向关闭，即主动关闭的一方A已不再发送数据。“对一个已经收到FIN包的socket调用read方法, 如果接收缓冲已空, 则返回0, 这就是常说的表示连接关闭。
但第一次对其调用write方法时, 如果发送缓冲没问题, 会返回正确写入(发送)。但发送的报文会导致对端发送RST报文, 因为对端的socket已经调用了close, 完全关闭, 既不发送, 也不接收数据。
所以, 第二次调用write方法(假设在收到RST之后), 会生成SIGPIPE信号, 导致进程退出。”可以使用SIGNAL(SIG_PIPE, SIG_IGN)忽略，或者注册一个handler函数处理。

本博客所有原创文章均采用[CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)许可协议。

您可以复制共享、演绎创作，但不得用于商业目的，转载请注明原创作者 **raining888** 。
