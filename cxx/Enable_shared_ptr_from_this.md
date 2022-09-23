使用智能指针难以避免的场景之一就是需要把 `this` 当成一个 `shared_ptr` 传递到其他函数中去。

你不能简单粗暴的用`this`指针构造一个`shared_ptr`，因为那样做会导致两个独立的 `shared_ptr` 指向同一份资源，会析构两次资源。

比如下面的代码:
```cpp
class A
{
  std::shared_ptr<A> getSharedPtr()
  {
    return shared_ptr<A>(this);     // 错误方式
  }
};

int main()
{
  auto a = std::make_shared<A>();
  a->getSharedPtr();
  return 0;
}
```

正确的方式是继承来自 `std::enable_shared_from_this`，然后调用它的 `shared_from_this` 成员函数，

> 注意：一定要`public`继承`std::enable_shared_from_this`否则会报`std::bad_weak_ptr` ！！！

```cpp
class A : public std::enable_shared_from_this<A>
{
  std::shared_ptr<A> getSharedPtr()
  {
      return shared_from_this();
  }
};

int main()
{
  auto widget = std::make_shared<Widget>();
  btn->getSharedPtr();
  return 0;
}
```
把自己作为基类的模板参数看起来非常诡异，它有一个更诡异的名字:**奇异递归模板模式**

## 如何共享 make_shared 的返回的资源

实际上，我们想要的不是基于 `this` 去创建一个 `shared_ptr`，而 是希望 `this` 的类型就是 `shared_ptr`。

想通了这一点你就会发现，其实我们应该 从 `shared_ptr` 类入手，而不是 `A` 类，因为只有 `shared_ptr` 类中 `this` 的类型才是 `shared_ptr`。

`make_shared` 会调用 `shared_ptr` 的构造函数，在 `shared_ptr` 的构造函数中，`this` 的类型就是 `shared_ptr`，所以从逻辑上分析，我们甚至可以这样去实现 `shared_from_this` 函数

```cpp
class shared_ptr<A>
{
public:
  explicit shared_ptr<A>(A* w)
  {
    // 其他初始化，可能在初始化列表中而不是构造函数内部
    setSharedPtr(a, this);
  }
};

void setSharedPtr(A* a, shared_ptr<A>* sp)
{
  // 建立 a 到 sp 的映射关系
}

void getSharedPtr(A* a)
{
  // 查找 a 对应的 sp
}

class A {
public:
  shared_ptr<A> shared_from_this()
  {
      getSharedPtr(this);
  }
};
```

## 抽象公共基类
上面的设计的最大的问题是，你需要在自己的内部定义一个 `shared_ptr` 类型的成员变量以 及提供一个 `getPtr` 和 `setSharedPtr` 成员方法，

这些代码和 A 类的业务逻辑可能没 有太大的关系，而且不适合重用。

为了抽象并重用这些代码，我们可以抽象出一个公共的基类，这也就是 `std::enable_shared_from_this` 实际上做的事情

```cpp
class enable_shared_from_this
{
public:
  void setSharedPtr(shared_ptr<A>* sp)
  {
      sp_ = *sp;
  }

  shared_ptr<A> getSharedPtr()
  {
      return sp_;
  }

private:
    shared_ptr<A> sp_;
};

class A : public enable_shared_from_this
{
};

class shared_ptr<A>
{
public:
    explicit shared_ptr<A>(A* a)
    {
         a->SetSharedPtr(this);
    }
};
```
上面这段代码最大的漏洞在于，`shared_ptr` 是一个模板，它并不知道 `A` 类是否继承 自 `enable_shared_from_this`，

所以 `a->setSharedPtr(this)` 这一句的调用不完全正确 。

boost 中解决这个问题使用了一个非常巧妙的方法：**重载**，通过重载我们可以让编译器自 动选择是否调用 `setSharedPtr`。
```cpp
class shared_ptr<A>
{
public:
  explicit shared_ptr<A>(A* a)
  {
    setSharedPtr(this, a);
  }
};

void setSharedPtr(shared_ptr<A>* sp, enable_shared_from_this* a)
{
  a->SetSharedPtr(sp);
}

void setSharedPtr(...)
{
  // nothing to do
}
```

## 避免循环引用
上面这些代码的逻辑上是正确的，但是实际上还有一个巨大的BUG，那就是 `A` 的内部 存在一个指向它自己的 `shared_ptr`，这意味着你永远无法销毁掉 `A`。

销毁 `A` 的前提是没有 `shared_ptr` 指向它了，而销毁 `A` 必然需要销毁它的成员变量 ，包括指向自身的那个 `shared_ptr`，而它的存在又否定了我们能销毁 `A` 的前提——没 有 `shared_ptr` 指向它。

这就像是你在画了一个指向自身的箭头，它让你自身形成了循环依赖，永远没有办法跳出来。

普通的 `shared_ptr` 循环依赖的问题的处理通常使用的是 `weak_ptr`，这一次也不例外。

我 们需要存储一个 `weak_ptr` 作为成员变量，而不是 `shared_ptr`，然后在需要的时候通过 `weak_ptr` 构建出 `shared_ptr`，所以正确的方式是：

```cpp
//伪代码
class enable_shared_from_this
{
public:
  setSharedPtr(shared_ptr* sp)
  {
    ap = sp; //伪代码，实际并不这么做
  }

  shared_ptr shared_from_this()
  {
    return shared_ptr(ap);   //这里把weak_ptr提升为share_ptr
  }

private:
  weak_ptr ap;
}
```
## 参考文献
1. Scott Meyers. [More Effective C++. 电子工业出版社, 2011](https://book.douban.com/subject/5908727/)
2. David Vandevoorde / Nicolai M.Josuttis. [C++ Templates. 人民邮电出版社, 2013](https://book.douban.com/subject/23780707/)
