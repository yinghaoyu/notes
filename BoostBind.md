## boost::bind实现分析
### 1. 函数对象

如果你不懂函数对象，请先了解一下函数对象

[《函数对象》](https://github.com/raining888/notes/blob/main/BookNotes.md#six-%E5%87%BD%E6%95%B0%E5%AF%B9%E8%B1%A1)

### 2. bind的参数

我们首先线想到`bind`参数分为两种，一种是用户创建`bind`的时候提供的`L`，另一种是 调用`bind`的`operator()()`的时候提供的`A`。

很显然，`L` 和 `A` 都可能有多个，多个 `A` 可以通过重载不同版本的 `operator()` 来解决，比 如：
```cpp
//以下为伪代码
template <typename F, typename L>
class BindImpl
{
public:
    template<typename A>
    operator()(A a);            // 单个参数

    template<typename A1, typename A2>
    operator()(A1 a1, A2 a2);   // 两个参数
    
    ...
    
private:
    F f_;
    L l_;
};
```
但是多个 `L` 却不行，因为**类是没有办法重载的**，你不能既定义 `BindImpl` 有两个模板参 数又定义它有三个模板参数。

而且就算你可以这么做，你可以以相同的方式处理 `L` ，使用模板的偏特化定义多个不同版本的 `BindImpl`，这种方式是一种不太合理的做法，因为 这样会导致 `L` 和 `A` 进行排列组合，实现起来将会极其复杂。

`boost`内部解决办法是归一， 增加一层间接性，使用 `List` 而不是使用单个元素。

```cpp
// 以下为伪代码
template<typename A1>
class List1 {
    // 省略了构造函数
private:
    A1 a1;
};

template<typename A1, typename A2>
class List2 {
    // 省略了构造函数
private:
    A1 a1;
    A2 a2;
}
```
这样一来我们就可以使用 `List1` 和 `List2` 作为 `BindImpl` 的参数从而解决 `L` 有多个的问题 。所以实际上，`bind` 这个函数的工作就是做 `List` 的封装以及对应 `BindImpl` 的创建。
```cpp
//以下为伪代码
template <typename F, A1, A2>
BindImpl<F, List2> bind(F f, A1 a1, A2 a2)
{
    List2 list(a1, a2);
    return BindImpl<F, List2>(f, list);
}
```
### 3. BindImpl 的 operator() 的实现

首先在 `BindImpl` 中，由于并不知道 `L` 到底是几个参数（就像你在 `vector` 的定义中不可能知道你存 储的到底是什么类型）。

所以没有办法在 `BindImpl` 中去处理参数绑定的问题，相反 **需要让 `L` 去处理参数的绑定问题**。

```cpp
//以下为伪代码
template<typename F, typename L>
class BindImpl
{
public:

    ...
    
    template<typename A>
    operator()(A a) {
        l_(f, a);
    }
    
    ...
    
private:
    F f_;
    L l_;
};
```
也就是说，List 也必须提供函数调用操作符。

```cpp
//以下为伪代码
template<typename A1, typename A2>
class List2
{
public:
    template<typename F, typename A>
    operator()(F f, A a);

private:
    A1 a1;
    A2 a2;
}
```

`BindImpl` 为了能够支持多个参数的调用，重载了多个 `operator()` 而这些重载的函数如果按照前面的实现方式——调用 `List` 对应的 `operator()` 的话就会导致 `List` 也需要重载多个 `operator` ，而这无疑是非常繁琐的事情。

为了解决这个问题，同样可以用列表替代单个参数。也就是像下面这样实现 `BindImpl` 的函数调用操作符。

```cpp
//以下为伪代码
template<typename F, typename L>
class BindImpl
{
public:
    template<typename A>
    operator()(A a)
    {
        List1 list(a);
        l_(f, list);
    }

    template<typename A1, typename A2>
    operator()(A1 a1, A2 a2)
    {
        List2 list(a1, a2);
        l_(f, list);
    }
private:
    F f_;
    L l_;
};
```
### 4. ListN 的 operator() 的实现

`ListN` 表示 `List1`，`List2`，`List3` 中的任何一个，这里以 `List2` 为例

`List2` 需要完成两件事情，完成参数的绑定和调用实际的函数 `f`。所以 `operator()` 最终看 起来应该是这个样子。

```cpp
//以下为伪代码
template<typename A1, typename A2>
class List2
{
public:
    template<typename F, typename L>
    operator()(F f, L l)
    {
        f(l1, l2);  // 这里的 l1，l2 是根据 函数参数L 推导出来的
    }

private:
    A1 a1;
    A2 a2
}
```
所以我们剩下的问题是如何根据 `l`，`a1`，`a2` 最终推导出 `f` 的实际调用参数 `l1`，`l2`

其实 这个算法很简单。以 `A1` 为例，如果 `A1` 是普通的值，那么 `l1 == a1`。

如果 `A1` 是一个占 位符，那么 `l1` 就等于 `l` 中对应的值。

`boost` 在实现这个逻辑的时候使用了一个非常巧妙 的方式——函数重载。它重载了 `List` 的 `[]` 操作符，然后根据参数的类型来判断返回什么值。

```cpp
//以下为伪代码
template<typename A1, typename A2>
class List2 {
public:
    template<typename F, typename L>
    operator()(F f, L l) {
        f(l[a1], l[a2]);  // 这里的根据 a1，a2 的类型得到实际的值
    }

private:
    A1 a1;
    A2 a2
}

template<typename A1>
class List1 {
public:
    A1 operator[](placeholder<1>) const { return a1; } // 如果是占位符
    template <typename T>
    T operator[](T v) const { return v; }   // 如果是普通的类型

private:
    A1 a1;
};
```
这里的实现使用了一个C++比较偏门的特性，**在重载解析的时候，普通函数的优先级高于模板函数**，也就是说当遇到类型为 placeholder<1> 的参数时候，虽然模板函数也可以实例化出正确的函数，但是因为有一个不需要要实例化的普通函数存在，重载解析会选择调用普通的函数，也就是调用返回占位符对应的值的那个函数。

### 5. placeholder<1> 的作用

从上面的代码我们可以看出，实际上，`placeholder<1>` 只是用来做重载解析的分派用的， 我们需要的是它的类型而不是它的值，所以你会发现前面 `operator[]` 甚至没有给出参数名称。`placeholder<1>` 的定义非常简单：

```cpp
template<int I>
class placeholder{};
placeholder<1> _1;
placeholder<2> _2;
placeholder<3> _3;
```
这种把数值当类型的技巧可以参考《C++设计新思维》一书。相信现在你应该很清楚 std::placeholder::_1 是什么东西了吧。

### 6. 如果实际调用的参数比绑定的参数多会怎么样？

boost::bind 有一个非常有意思的特性那就是你可以提供比绑定的参数更多的实际参数，多出来的这些参数会被自动忽略掉。

```cpp
int fun(int a)
{
    return a;
}

bind_t<int(*)(int), List1<placeholder<1> > > binder(fun, _1);

binder(20);
binder(20, 30);  // 这个调用是合法的！
```

为什么呢？因为`binder`根本不知道2这个参数的存在。如前所述，`binder`的`L`是 `List1`（因为绑定的时候只有 `std::placeholder::_1` 这个参数）：

所以当我们调用`binder(1, 2)`的时候调用的是`List1`的`operator()(F f, A a)`成员函 数，这里的`f`是`fun`，而`a`是我们提供的实际调用参数`List2（1, 2）`。

实际上 `List1` 的 `operator()()` 实现如下：

```cpp
template<typename F, typename L>
operator()(F f, L l)
{
    f(l[a1]);   // a1 就是 std::placeholder::_1
}
```
上面这个调用最终变成了`fun(20)`，也就是说 `30` 这个参数默默的被吞掉了。

###  7. 忽略参数的作用

这种看似诡异的行为其实有它独特的作用，那就是选择性的忽略掉你不想处理参数，比如你 有一个回调函数的原型如下：

```cpp
boost::function<void(int, double)>;
```
那么你在设置这个回调的时候可以使用下面这个`binder`：
```cpp
void foo(int);
boost::bind(foo, std::placeholder::_1);
```
也可以使用下面这个`binder`：
```cpp
void bar(int, double);
boost::bind(bar, std::placeholder::_1, std::placeholder::_2);
```
这种灵活性使用`lambda`好像没有办法实现。

## 8. 源码

1. [《bind贴合本文的简单实现》](https://github.com/raining888/notes/blob/main/src/bind_t.cc)

2. [《bind的简单实现2》](https://github.com/raining888/notes/blob/main/src/strict_bind.cc)

## 参考文献
