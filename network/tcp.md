![tcp1](/images/tcp1.png)

MSL:Maximum Segment Lifetime

1. TCP总共有几种状态？

> 11种。分别为CLOSED, SYN_SEND, SYN_RECVD, ESTABLISH, LISTEN, FIN_WAIT-1, CLOSE_WAIT, FIN_WAIT_2, LAST_ACK, TIME_WAIT, CLOSING。其中CLOSING状态是A、B两端同时发起close时，产生的中间状态。

![tcp2](/images/tcp2.png)

2. 如何解决过多的TIME_WAIT状态？

> TIME_WAIT一般是主动发起close方的状态，可以对socket设置SO_REUSEADDR选项。

3. B端进程为什么会触发SIG_PIPE信号？以及如何解决？

> “对一个已经收到FIN包的socket调用read方法, 如果接收缓冲已空, 则返回0, 这就是常说的表示连接关闭。
但第一次对其调用write方法时, 如果发送缓冲没问题, 会返回正确写入(发送)。但发送的报文会导致对端发送RST报文, 因为对端的socket已经调用了close, **完全关闭, 既不发送, 也不接收数据**。
所以, 第二次调用write方法(假设在收到RST之后), 会生成SIGPIPE信号, 导致进程退出。”可以使用SIGNAL(SIG_PIPE, SIG_IGN)忽略，或者注册一个handler函数处理。

4. 什么时候会发送FIN包？

> close后，socket**引用计数为0**，会发送FIN包，否则不会发送。因为有可能子进程继承了父进程的fd，没有对socket设置SOCK_CLOEXEC选项。

5. 什么时候会发送RST包？

> 主要有三种情况。一，建立连接的SYN到达某端口，但是该端口上没有正在监听的服务。二，TCP收到了一个根本不存在的连接上的分节，例如主机崩溃后重启，它的TCP丢失了崩溃前的所有连接信息。
三，请求超时，例如KeepAlive超时，会发送RST包。

6. showdown和close有什么区别？

> close不管输出缓冲区中是否还有数据，而shutdown会等输出缓冲区中的数据传输完毕再发送FIN包。也就意味着，调用close将丢失输出缓冲区中的数据，而调用shutdown不会。shotdown可以单向关闭写或读（half-close）。

7. 什么情况下出现TCP自连接？

> 在发起连接时，TCP/IP的协议栈会先选择source IP和source port，在没有显示调用bind的情况下，source IP由路由表确定，source port由TCP/IP协议栈从local port range中选取尚未使用的port。如果destination IP正好是本机，而destination port位于local port range，且没有服务器监听(listen)的话，source port可能正好选中了destination port，这就出现了（source IP，source port）=（destination IP，destination port）的情况，即发生了自连接。

8. 为什么要采用三次握手，而不是两次握手？

> 第一，三次握手每次都是接收到数据包的一方可以得知对方的情况，但发送方其实没有任何头绪。我虽然有发包的动作，但是我怎么知道我有没有发出去，而对方有没有接收到呢？所以经历了上面的三次握手过程，客户端和服务端都确认了自己的接收、发送能力是正常的。之后就可以正常通信了。第二，为了**防止已失效的连接请求报文段突然又传送到服务器，产生错误**。已失效的连接请求报文段的产生原因：当客户A发送连接请求，但因连接请求报文丢失而未收到确认。于是A会再次重传一次连接请求，此时服务器端B收到再次重传的连接请求，建立了连接，然后进行数据传输，数据传输完了后，就释放了此连接。假设A第一次发送的连接请求并没有丢失，而是在网络结点中滞留了太长时间，以致在AB通信完后，才到达B。此时这个连接请求其实已经是被A认为丢失的了。如果不进行第三次握手，那么服务器B可能在收到这个已失效的连接请求后，进行确认，然后单方面进入ESTABLISHED状态，而A此时并不会对B的确认进行理睬，这样就白白的浪费了服务器的资源。
