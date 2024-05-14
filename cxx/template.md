摘自[模板不仅仅是模板，它还能解耦](http://www.purecpp.cn/detail?id=2403)

在很多人眼里的模板就是用于泛型编程的场景，甚至有些讨厌它，可能很多人还不知道它还有另外一个妙用--解耦。

看这样一个场景：
```cpp
class connection {
public:
  void send() {
    //do business
    std::cout<<buf_<<"\n";
    //...
  }

private:
  std::string buf_;
};
```
有这样一个对象connection，它里面有个send函数，send的逻辑比较复杂(这里省略具体逻辑)，然后这部分代码刚好可以被另外一个对象A复用，于是我就把send的业务逻辑封装到另外一个helper对象中，helper对象需要访问connection对象的buf_字段，而A对象也有这样一个buf_字段，所以connection和A对象都能调用helper的send方法，从而达到复用的目的。
```cpp
#include "helper.hpp"
class connection {
public:
  void send() {
    helper help(this);
    help.send();
  }

private:
  friend class helper;
  std::string buf_;
};
```
把helper设置为friend是为了让helper访问私有字段buf_。再看看helper的代码。
```cpp
#include "connection.hpp"

class helper {
public:
  helper(connection *conn) : buf_(conn->buf_) {}
  void send() { std::cout << buf_ << "\n"; }

private:
  std::string& buf_;
};
```
但是这个代码这样写是编译不过的，因为两个对象依赖相互引用头文件了。不管用什么方法，相互依赖总是不好的，好办法是破除这个依赖。

是时候叫出模板了！
```cpp
template<typename T>
class helper {
public:
  helper(T *t) : buf_(t->buf_) {}
  void send() { std::cout << buf_ << "\n"; }

private:
  std::string& buf_;
};
```
helper 不再引用connection的头文件，而是通过一个模板来泛化含有buf_的对象，它不再关心具体的对象是什么了，它只关心这个对象是否存在buf_字段。
```cpp
#include "helper.hpp"
class connection {
public:
  void send() {
    helper help(this);
    help.send();
  }

private:
  friend class helper<connection>;
  std::string buf_;
};
#include "helper.hpp"
class A {
public:
  void send() {
    helper help(this);
    help.send();
  }

private:
  friend class helper<A>;
  std::string buf_;
};
```
低版本的编译器可能在这一行代码编译不过：

helper help(this);
因为helper是一个模板类，需要填模板参数。这时候可以使用C++17的dedution guide来消除这个模板参数：
```cpp
template<typename T>
class helper {
public:
  helper(T *t) : buf_(t->buf_) {}
  void send() { std::cout << buf_ << "\n"; }

private:
  std::string& buf_;
};

template <typename T>
helper(T *t) -> helper<T>; //dedution guide
```
这样connection和A就能复用同样的逻辑了，而且也不会产生相互依赖，解开了耦合。模板不仅仅是为了泛化，有时候可以用来解耦。
