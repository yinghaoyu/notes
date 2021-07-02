## NRVO基本思想

命名的返回值优化（**Named Return Value Optimization**），这优化了冗余拷贝构造函数和析构函数调用，从而提高了总体性能。

下面是一个简单示例，以说明优化及其实现方式：
```cpp
A Fun ()
{
  A a;
  return a;
}
```
使用上述函数的程序可能具有如下构造：
```cpp
a = fun();
```
从fun返回的值是在`a`通过使用隐藏的参数所指向的内存空间中创建的。
```cpp
// 以下代码为不使用NRVO的伪代码
A fun(A &_hiddenArg)
{
  A a;
  a.A::A();  // ctor for a
  _hiddenArg.A::A(a);  // the copy ctor for _hiddenArg
  a.A::~A();  // dtor for a
  return;
}
```
从上面的代码可以看出，有一些优化的机会。其基本思想是消除基于堆栈的临时值（a）并使用隐藏的参数。因此，这将消除基于堆栈的值的拷贝构造函数和析构函数。下面是基于NRVO的优化代码：
```cpp
A fun(A &_hiddenArg)
{
  _hiddenArg.A::A();
  return;
}
```

## 优化限制

- 在遇到异常时，隐藏的参数必须在它正在替换的临时范围内被破坏

```cpp
class RVO
{
public:
  RVO() { printf("ctor\n"); }
  RVO( const RVO& c_RVO ) { printf("copy ctor\n"); }
  ~RVO() { printf("dtor\n"); }
};

RVO fun()
{
  RVO rvo;
  throw "I am throwing an exception!";
  return rvo;
}

int main()
{
  RVO rvo;
  try
  {
    rvo = fun();
  }
  catch(char* str)
  {
    printf("I caught the exception\n");
  }
}
```

预期输出
```cpp
ctor
ctor
dtor
I am throwing an exception!
dtor
```
注释掉`throw`，输出
```
ctor
ctor
copy ctor
dtor
dtor
dtor
```
注释掉`throw`，并启用`NRVO`，输出
```
ctor
ctor
dtor
dtor
```

- 若要使用优化，所有退出路径必须返回同一命名对象

```cpp
RVO fun()
{
  RVO rvo;
  if（condition）
  {
    return RVO();
  }
  return rvo;
}

int main()
{
  RVO rvo;
  rvo = fun();
}
```
输出
```
ctor
ctor
copy ctor
dtor
dtor
dtor
```
启用优化时输出与不启用任何优化相同。`NRVO`实际上并不发生，因为并非所有返回都返回相同的对象。
