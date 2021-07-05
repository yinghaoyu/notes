关于C++的拷贝构造函数，很多的建议是直接禁用。为什么呢？没有拷贝构造函数会有什么限制呢？如何禁用拷贝构造呢？这篇文章对这些问题做一个简单的总结。

这里讨论的问题以拷贝构造函数为例子，但是通常赋值操作符是通过拷贝构造函数来实现的（`copy-and-swap`），所以这里讨论也适用于赋值操作符，通常来说禁用拷贝构造函数的同时也会禁用赋值操作符。

## 为什么禁用拷贝构造函数
关于拷贝构造函数的禁用原因，我目前了解的主要是两个原因。第一是**浅拷贝**问题，第二个则是**基类拷贝**问题。

## 浅拷贝问题
编译器默认生成的构造函数，是`memberwise`拷贝，也就是逐个拷贝成员变量（一般为浅拷贝），对于下面这个类的定义：
```cpp
class A
{
 public:
  A(const std::string &name) : name_(name), buf_(new char[10]) {}
  ~A() { delete buf_; }

 private:
  std::string name_;
  char *buf_;
};
```
默认生成的拷贝构造函数，会直接拷贝`buf_`的值，导致两个A对象指向同一个缓冲区，这会导致析构的时候两次删除同一片区域的问题（这个问题又叫**双杀问题**）。

解决这个问题的方式有很多：

自己编写拷贝构造函数，然后在拷贝构造函数中创建新的`buf_`，不过拷贝构造函数的 编写需要考虑异常安全的问题，所以编写起来有一定的难度。

使用 `shared_ptr` 这样的智能指针，让所有的 `A` 对象共享一片 `buf_`，并 让 `shared_ptr` 的引用计数机制帮你智能的处理删除问题。

禁用拷贝构造函数和赋值操作符。如果你根本没有打算让`A`支持拷贝，你完全可以直接禁用这两操作，这样一来，前面提到的这些问题就都不是问题了。

## 基类拷贝构造问题
如果我们不去自己编写拷贝构造函数，编译器默认生成的版本会自动调用基类的拷贝构造 函数完成基类的拷贝：
```cpp
class Base
{
 public:
  Base() { cout << "Base Default Ctor" << endl; }
  Base(const Base &) { cout << "Base Copy Ctor" << endl; }
};

class Drived : public Base
{
 public:
  Drived() { cout << "Drived Default Ctor" << endl; }
};

int main(void)
{
  Drived d1;
  Drived d2(d1);
}
```
上面这段代码的输出如下：
```cpp
Base Default Ctor
Drived Default Ctor

Base Copy Ctor  // 自动调用了基类的拷贝构造函数
```
但是如果我们出于某种原因编写了，自己编写了拷贝构造函数（比如因为上文中提到的浅拷贝问题），编译器**不会帮我们安插基类的拷贝构造函数**，它只会在必要的时候**帮我们安插基类的默认构造函数**：
```cpp
class Base {
 public:
  Base() { cout << "Base Default CtorCtorCtorCtor" << endl; }
  Base(const Base &) { cout << "Base Copy CtorCtorCtor" << endl; }
};

class Drived : public Base
{
 public:
  Drived() { cout << "Drived Default CtorCtor" << endl; }
  Drived(const Drived& d)
  {
   cout << "Drived Copy Ctor" << endl;
  }
};

int main(void)
{
  Drived d1;
  Drived d2(d1);
}
```
上面这段代码的输出如下：
```cpp
Base Default Ctor
Drived Default Ctor

Base Default Ctor // 调用了基类的默认构造函数
Drived Copy Ctor
这当然不是我们想要看到的结果，为了能够得到正确的结果，我们需要自己手动调用基类 的对应版本拷贝基类对象。

Drived(const Drived& d) : Base(d)  // 在这里显示调用基类的拷贝构造函数
{
  cout << "Drived Copy Ctor" << endl;
}
```
这本来不是什么问题，只不过有些人编写拷贝构造函数的时候会忘记这一点，所以导致基类子对象没有正常复制，造成很难察觉的`BUG`。所以为了一劳永逸的解决这些蛋疼的问题， 干脆就直接禁用拷贝构造和赋值操作符。

## 没有拷贝构造的限制
在`C++11`之前对象必须有正常的拷贝语义才能放入容器中，禁用拷贝构造的对象无法直接放入容器中，当然你可以使用指针来规避这一点，但是你又落入了自己管理指针的困境之中 （或许使用智能指针可以缓解这一问题）。

`C++11`中存在移动语义，你可以通过移动而不是拷贝把数据放入容器中。

拷贝构造函数的另一个应用在于设计模式中的原型模式，在`C++`中没有拷贝构造函数，这个模式实现可能比较困难。

## 如何禁用拷贝构造
如果你的编译器支持`C++11`，直接使用`delete`

否则你可以把拷贝构造函数和赋值操作符声明成private同时不提供实现。

你可以通过一个基类来封装第二步，因为默认生成的拷贝构造函数会自动调用基类的拷 贝构造函数，如果基类的拷贝构造函数是 private，那么它无法访问，也就无法正常 生成拷贝构造函数。

```cpp
class NonCopyable
{
protected:
  NonCopyable() {}
  ~NonCopyable() {}  // 构造函数和析构函数设置protected权限，这样子类可以调用，但是外面的类不能调用，那么当子类需要定义构造函数的时候不至于通不过编译。
private:
    NonCopyable(const NonCopyable&);  // 把拷贝构造函数和拷贝赋值函数做成了private的，继承自noncopyable的类在执行拷贝操作时会调用基类的拷贝操作，但是基类的拷贝操作是private的，因此无法调用，引发编译错误。
    NonCopyable& operator=(const NonCopyable&);
};

class A : private NonCopyable  // 基类的成员全部变为private
{
};

A a(A()); // 错误
```
上不会生成memberwise的拷贝构造函数。

## 参考文献
