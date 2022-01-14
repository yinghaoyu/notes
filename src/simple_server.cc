#include <iostream> // std::cout、std::endl
#include <sys/socket.h> // socket、bind、listen、accept
#include <netinet/in.h> // htonl、htons
#include <unistd.h> // write、close
#include <string.h> // bzero

#define PORT 8099
#define LISTENQ 5

int main(){
  /* 准备重要参数 */
  // 结构体 sockaddr_in 位于 netinet/in.h
  int listenfd, connfd;
  struct sockaddr_in servaddr;
  /* 创建套接字 */
  // 常量 AF_INET 位于 bits/socket.h，经由 sys/socket.h 导入
  // 常量 SOCK_STREAM 位于 bits/socket_type.h，先后经由 bits/socket.h、sys/socket.h 导入
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if(listenfd < 0){
    std::cout << "create socketfd failed" <<std::endl;
    return 0;
  }
  /* 准备地址端口信息 */
  // 常量 INADDR_ANY 位于 netinet/in.h
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);
  /* 绑定套接字与地址端口 */
  int bind_ok = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if(bind_ok < 0){
    std::cout << "bind socket with server address failed" <<std::endl;
    return 0;
  }
  /* 监听端口 */
  int listen_ok = listen(listenfd, LISTENQ);
  if(listen_ok < 0){
    std::cout << "listen socket failed" <<std::endl;
    return 0;
  }
  /* 准备这个简单服务器返回的信息 */
  std::string buffer = "hello, this is a simple server.\n";
  int write_ok; // 用于判断是否成功将数据写入发送缓存
  // 循环等待
  while(1){
    // 监听到有请求，创建交互的套接字
    connfd = accept(listenfd, NULL, NULL);
    if(connfd < 0){
      std::cout << "create connection socket failed" <<std::endl;
      return 0;
    }
    std::cout << buffer << std::endl << "length of it: " << buffer.size()<< std::endl;
    // 将响应的数据写入发送缓冲区
    write_ok = write(connfd, buffer.c_str(), strlen(buffer.c_str()));
    if(write_ok < 0){
      std::cout << "write info to connection socket failed" <<std::endl;
      close(connfd); // 关闭连接
      return 0;
    }
    // 关闭连接
    close(connfd);
  }
}
