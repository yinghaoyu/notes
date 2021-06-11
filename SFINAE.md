SFINAE，即匹配失败不是错误(Substitution Failure Is Not An Error)。

- “Error”：一般意义上的编译错误。一旦出现编译错误，大家都知道，编译器就会中止编译，并且停止接下来的代码生成和链接等后续活动。

- “Failure”：很多时候光看字面意思，很多人会把 Failure 和 Error 等同起来。但是实际上Failure很多场合下只是一个中性词。比如我们看下面这个虚构的例子就知道这两者的区别了。

- “substitution”：将函数模板中的形参，替换成实参的过程。

在决议重载的模板函数或者特化/偏特化类的时候，如果对一个较为「特化」版的候选者的模板形参推断（匹配）失败，将转而对其他次「特化」的候选者进行模板参数推断（匹配），而不是返回一个错误。

看一个简单的例子：
```cpp
struct Test
{
    typedef int foo;
};

template<typename T>
void bar(typename T::foo) // #1
{
}

template<typename T>
void bar(T) // #2
{
}

int main(int argc, char* argv[])
{
  bar<Test>(0); // call #1 succ
  bar<int>(0);  // call #1 failure, call #2 succ, not error
}
```

第一个函数调用，显示实例化形参为Test类型，而Test有一个类型foo，所以匹配#1版本成功。

第二个函数调用，因为编译器总是最先尝试推断（匹配）「最特化」的候选函数，所以先对#1版本进行匹配，而int是一个内置类型，并没有子类型foo，所以#1版本匹配失败，然后转而对#2版本的候选函数进行匹配，即T实例化为int类型，匹配成功。
