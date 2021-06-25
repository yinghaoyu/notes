基本目标如下：

- 支持接收0个参数的函数/函数指针，函数对象。

- 支持接收1个参数的函数/函数指针，函数对象。

- 支持接收2个参数的函数/函数指针，函数对象。

### 实现

首先，解决占位符的问题：

```cpp
namespace
{

struct Placeholders1
{
} _1;

struct Placeholders2
{
} _2;

}
```

使用匿名namespace的原因是防止不同编译单元中的命名冲突， 让占位符对象只在其所在的编译单元中可见。

在boost::bind源码中主要是通过2个list表维持各种相关信息。一个bindlist表维持了bind接收的绑定参数，包括占位符，用户传入的变量等。一个calllist维持了调用bind返回的对象时所传入的参数信息。它们的通过继承层次的方式来表现的。

下面这个继承层次的每一个类都要作为对应的bindlist和calllist层次中的基类，它们分别保存了bind接收的绑定参数信息（用户传入的变量，占位符），以及调用bind返回的对象时所传入的参数信息。使用匿名namespace的原因是防止不同编译单元中的命名冲突， 让占位符对象只在其所在的编译单元中可见。
在boost::bind源码中主要是通过2个list表维持各种相关信息。一个bindlist表维持了bind接收的绑定参数，包括占位符，用户传入的变量等。一个calllist维持了调用bind返回的对象时所传入的参数信息。它们的通过继承层次的方式来表现的。

下面这个继承层次的每一个类都要作为对应的bindlist和calllist层次中的基类，它们分别保存了bind接收的绑定参数信息（用户传入的变量，占位符），以及调用bind返回的对象时所传入的参数信息。

```cpp
class Base0
{
};

template<typename T1>
class Base1 : public Base0
{
public:
    Base1(T1 data1)
        : data1_(data1)
    {
    }

protected:
    T1 data1_;
};

template<typename T1, typename T2>
class Base2 : public Base1<T1>
{
public:
    Base2(T1 data1, T2 data2)
        : Base1<T1>(data1), data2_(data2)
    {
    }

protected:
    T2 data2_;
};
```

接着，就是所谓的calllist的实现了。它们的基类将保存调用bind返回的对象时所传入的参数信息。
```cpp
class CallList0 : public Base0
{
public:
    template<typename _T>
    _T operator[](_T arg)
    {
        return arg;
    }
};

template<typename T1>
class CallList1 : public Base1<T1>
{
public:
    CallList1(T1 data1)
        : Base1<T1>(data1)
    {
    }

    T1 operator[](Placeholders1 arg1)
    {
        return Base1<T1>::data1_;
    }

    template<typename _T>
    _T operator[](_T arg)
    {
        return arg;
    }

};

template<typename T1, typename T2>
class CallList2: public Base2<T1, T2>
{
public:
    CallList2(T1 data1, T2 data2)
        : Base2<T1, T2>(data1, data2)
    {
    }

    T1 operator[](Placeholders1 arg1)
    {
        return Base2<T1, T2>::data1_;
    }

    T2 operator[](Placeholders2 arg2)
    {
        return Base2<T1, T2>::data2_;
    }

    template<typename _T>
    _T operator[](_T arg)
    {
        return arg;
    }
};
```

然后，我们来看看bindlist，它们的基类主要保存了bind接收的占位符、参数信息。
```cpp
class BindLinst0 : public Base0
{
public:
    template<typename Func>
    void operator()(Func func)
    {
        func();
    }
};

template<typename T1>
class BindList1 : public Base1<T1>
{
public:
    BindList1(T1 data1)
        : Base1<T1>(data1)
    {
    }

    template<typename Func, typename Call>
    void operator()(Func func, Call call)
    {
        func(call[Base1<T1>::data1_]);
    }
};

template<typename T1, typename T2>
class BindList2 : public Base2<T1, T2>
{
public:
    BindList2(T1 data1, T2 data2)
        : Base2<T1, T2>(data1, data2)
    {
    }

    template<typename Func, typename Call>
    void operator()(Func func, Call call)
    {
        func(call[Base2<T1, T2>::data1_], call[Base2<T1, T2>::data2_]);
    }
};
```

接下来就是bind函数所返回的对象了，相信童鞋们可以想象的到，这个对象中应该主要保存的是bind函数接收的参数信息咯，并且他还保存了所注册的函数。
```cpp
template<typename Func, typename Bind>
class BindImpl
{
public:
    BindImpl(Func func, Bind bindlist)
        : func_(func), bindlist_(bindlist)
    {
    }

    void operator()()
    {
        bindlist_(func_);
    }

    template<typename T1>
    void operator()(T1 data1)
    {
        bindlist_(func_, CallList1<T1>(data1));
    }

    template<typename T1, typename T2>
    void operator()(T1 data1, T2 data2)
    {
        bindlist_(func_, CallList2<T1, T2>(data1, data2));
    }

protected:
    Func func_;
    Bind bindlist_;
};
```
如此，基本的轮廓就已经出来了。bind函数返回一个BindImpl对象，里面保存了注册的函数和bind接收的占位符、参数信息。当我们调用这个对象的时候，会生成一个calllist对象，它保存了调用BindImpl对象时所传入的参数，然后在bindlist中调用注册的函数。
需要的注意的是，在bindlist调用函数时我们转而调用了calllist的operator[]函数，通过它来判断传入的参数是占位符还是用户传入的参数，如果是占位符，那么就返回calllist中保存的之前注册的用户传入的信息。如果不是占位符，operator[]函数就单纯的返回他接收的参数，也就是之前用户调用BindImpl时传入的参数。

最后，我们通过一组重载的bind函数来实现对接收0个参数、1个参数、2个参数的支持，它们返回的是一个BindImpl对象。
```cpp
template<typename Func>
BindImpl<Func, BindLinst0> bind(Func func)
{
    return BindImpl<Func, BindLinst0>(func, BindLinst0());
}

template<typename Func, typename T1>
BindImpl<Func, BindList1<T1> > bind(Func func, T1 data1)
{
    return BindImpl<Func, BindList1<T1> >(func, BindList1<T1>(data1));
}

template<typename Func, typename T1, typename T2>
BindImpl<Func, BindList2<T1, T2> > bind(Func func, T1 data1, T2 data2)
{
    return BindImpl<Func, BindList2<T1, T2> >(func, BindList2<T1, T2>(data1, data2));
}
```
这样bind函数的基本功能就实现了，但是需要提到的是，目前的bind并不支持注册成员函数。如果要支持成员函数注册的话，需要萃取函数指针的返回类型以及每个参数类型，具体方法在上一篇blog《boost::function的简单实现》中介绍到了，有兴趣的童鞋可以去看看。
下面就来简单的测试一下：
```cpp
int get(int a, int b)
{
    std::cout << a + b << std::endl;
    return 0;
}

class Point
{
public:
    int operator()(int a, int b)
    {
        std::cout << "Point::operator() called: a = " << a + b << std::endl;
        return a + b;
    }
};

int main(int argc, char const *argv[])
{
    bind(get, _1, 10)(20, 1);
    bind(Point(), _1, _2)(3, 4);
    return 0;
}
```
结果为：
```cpp
30
Point::operator() called: a = 7
```
