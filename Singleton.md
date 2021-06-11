## 单线程模式的Singleton

```cpp
template<typename T>
class Singleton
{
public:
  static T& getInstance()
  {
    if (!value_)
    {
      value_ = new T();  // not thread safe
    }
    return *value_;
  }

private:
  Singleton();
  ~Singleton();

  static T* value_;
};

template<typename T>
T* Singleton<T>::value_ = NULL;  // 全局静态变量
```

## 多线程模式下的Singleton

上面单线程版本存在race condition，从而产生**多次初始化**的情况

```cpp
template<typename T>
class Singleton
{
public:
  static T& getInstance()
  {
    {
      MutexGuard guard(mutex_); // RAII
      if (!value_)
      {
        value_ = new T();  //thread safe now, but ineffective when heavy contention
      }
    }
    return *value_;
  }
  
private:
  Singleton();
  ~Singleton();
  
  static T* value_;
  static Mutex  mutex_;
};

template<typename T>
T* Singleton<T>::value_ = NULL;  // 全局静态变量
```

## DCL版本的Singleton

DCL即double-checked locking。在普通加锁的写法中，每次调用getInstance都会进入临界区，这样在heavy contention的情况下该函数就会成为系统性能的瓶颈，
这个时候就有先驱者们想到了DCL写法，也就是进行**两次check**，当第一次check为假时，才加锁进行第二次check

```cpp
template<typename T>
class Singleton
{
public:
  static T& getInstance()
  {
    if(!value_)
    {
      MutexGuard guard(mutex_); // RAII
      if (!value_)
      {
        value_ = new T();  //
      }
    }
    return *value_;
  }
  
private:
  Singleton();
  ~Singleton();
  
  static T* value_;
  static Mutex  mutex_;
};

template<typename T>
T* Singleton<T>::value_ = NULL;  // 全局静态变量
```

在一段时间内，大家都以为这是正确的、有效的做法。实际上却不是这样的。

错误在哪呢？我们先看value_ = new T这一句发生了什么：

1. 分配了一个T类型对象所需要的内存。

2. 在分配的内存处构造T类型的对象。

3. 把分配的内存的地址赋给指针value_。

主观上，我们会觉得计算机在会按照1、2、3的步骤来执行代码，但是问题就出在这。实际上只能确定步骤1最先执行，而步骤2、3的执行顺序却是不一定的。

假如某一个线程a在调用getInstance的时候按照1、3、2的步骤执行，那么当刚刚执行完步骤3的时候发生线程切换，计算机开始执行另外一个线程b。

因为第一次check没有上锁保护，那么在线程b中调用getInstance的时候，不会在第一次check上等待，而是执行这一句，那么此时value_已经被赋值了，就会直接返回`*value_`然后执行后面使用t类型对象的语句，但是在a线程中步骤3还没有执行！

也就是说在b线程中通过getInstance**返回的对象还没有被构造**就被拿去使用了！这样就会发生一些难以debug的灾难问题。

volatile关键字也不会影响执行顺序的不确定性。

在多核机器的环境下，2个核同时执行上面的a、b两个线程时，由于第一次check没有锁保护，依然会出现使用实际没有被构造的对象的情况。

## Meyers Singleton

Scott Meyer在《Effective C++》中提出了一种简洁的Singleton写法

```cpp
template<typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T value;
        return value;
    }

private:
    Singleton();
    ~Singleton();
};
```

先说结论：

1. 单线程下，正确。

2. C++11及以后的版本（如C++14）的多线程下，正确。

3. C++11之前的多线程下，不一定正确。

原因在于在c++11之前的标准中并没有规定local static变量的内存模型，所以很多编译器在实现local static变量的时候仅仅是进行了一次check（参考《深入探索C++对象模型》），
于是getInstance函数被编译器改写成这样了：

```cpp
bool initialized = false;
char value[sizeof(T)];

T& getInstance()
{
  if (!initialized)
  {
     initialized = true;
     new (value) T();
  }
  return *(reinterpret_cast<T*>(value));
}
```

于是乎它就是不是线程安全的了。

但是在C++11却是线程安全的，这是因为新的C++标准规定了当一个线程正在初始化一个变量的时候，其他线程必须得等到该初始化完成以后才能访问它。

在C++11 standard中：

> if control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.

## pthread_once版本的Singleton

如果是在unix平台的话，在不适用C++11的情况下，可以通过pthread_once来实现singleton。

pthread_once的原型为

```cpp
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void))
```
AUPE（《Unix环境编程》）中对于pthread_once是这样说的：

如果每个线程都调用pthread_once，系统就能保证初始化例程init_routine只被调用一次，即在系统首次调用pthread_once时。

以下源码摘自陈硕muduo网络库
```cpp
template<typename T>
class Singleton : boost::noncopyable
{
 public:
  static T& instance()   //得到对象
  {
    pthread_once(&ponce_, &Singleton::init);   //第一次调用会在init函数内部创建，pthread_once保证该函数只被调用一次！！！！
    									   //并且pthread_once()能保证线程安全，效率高于mutex
    assert(value_ != NULL);
    return *value_;    //利用pthread_once只构造一次对象
  }
 
 private:
  Singleton();
  ~Singleton();
 
  static void init()   //客户端初始化该类
  {
    value_ = new T();   //直接调用构造函数
    if (!detail::has_no_destroy<T>::value)   //当参数是类且没有"no_destroy"方法才会注册atexit的destroy
    {
      ::atexit(destroy);   //登记atexit时调用的销毁函数，防止内存泄漏
    }
  }
 
  static void destroy()  //程序结束后自动调用该函数销毁
  {
    //用typedef定义了一个数组类型，数组的大小不能为-1，利用这个方法，如果是不完全类型，编译阶段就会发现错误
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];  //要销毁这个类型，这个类型必须是完全类型
    T_must_be_complete_type dummy; (void) dummy;  //这个
 
    delete value_;   //销毁
    value_ = NULL;   //赋空
  }
 
 private:
  static pthread_once_t ponce_;     //pthread_once的参数
  static T*             value_;        //模板T类型的指针
};
 
template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;   //初始化pthread_once
 
template<typename T>
T* Singleton<T>::value_ = NULL;    //静态成员外部会初始化为空
```

## static object版本的Singleton

```cpp
template<typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        return *value_;
    }

private:
    Singleton();
    ~Singleton();

    class Helper
    {
    public:
        Helper()
        {
            Singleton<T>::value_ = new T();
        }

        ~Helper()
        {
            delete value_;
            value_ = NULL;
        }
    };

    friend class Helper;

    static T*      value_;
    static Helper  helper_;
};

template<typename T>
T* Singleton<T>::value_ = NULL;

template<typename T>
typename Singleton<T>::Helper Singleton<T>::helper_;
```

在进入main之前就把singleton对象构造出来就可以避免在进入main函数后的多线程环境中构造的各种情况了。

这种写法有一个前提就是**不能在main函数执行之前调用getInstance**，因为c++标准只保证静态变量在main函数之前之前被构造完成。

可能有人会说如果helper的初始化先于value_初始化的话，那么helper_初始化的时候就会使用尚没有被初始化的value_，这个时候使用其返回的对象就会出现问题，或者在后面value_**真正**初始化的时候会覆盖掉helper_初始化时赋给value_的值。

实际上这种情况不会发生，value_的初始化一定先于helper_，因为C++标准保证了这一行为：

> the storage for objects with static storage duration (basic.stc.static) shall be zero-initialized (dcl.init) before any other initialization takes place. zero-initialization and initialization with a constant expression are collectively called static initialization; all other initialization is dynamic initialization. objects of pod types (basic.types) with static storage duration initialized with constant expressions (expr.const) shall be initialized before any dynamic initialization takes place. objects with static storage duration defined in namespace scope in the same translation unit and dynamically initialized shall be initialized in the order in which their definition appears in the translation unit.

## local static
上面一种写法只能在进入main函数后才能调用getInstance，那么有人说，我要在main函数之前调用怎么办？

这个时候我们就可以利用local static来实现，C++标准保证函数内的local static变量在函数调用之前被初始化构造完成，利用这一特性就可以达到目的：

```cpp
template<typename T>
class Singleton
{
private:
    Singleton();
    ~Singleton();

    class Creater
    {
    public:
        Creater()
            : value_(new T())
        {
        }

        ~Creater()
        {
            delete value_;
            value_ = NULL;
        }

        T& getValue()
        {
            return *value_;
        }

        T* value_;
    };

public:
    static T& getInstance()
    {
        static Creater creater;
        return creater.getValue();
    }

private:
    class Dummy
    {
    public:
        Dummy()
        {
            Singleton<T>::getInstance();
        }
    };

    static Dummy dummy_;
};

template<typename T>
typename Singleton<T>::Dummy Singleton<T>::dummy_;
```

这样就可以了。dummy_的作用是即使在main函数之前没有调用getInstance，它依然会**作为最后一道屏障保证在进入main函数之前构造完成Singleton对象**。

这样就避免了在进入main函数后的多线程环境中初始化的各种问题了。

但是此种方法**只能在main函数执行之前的环境是单线程的环境下**才能正确工作。

## 总结

实际上，上文所讲述了各种写法中，有一些不能在main函数之前调用。有一些可以在main函数之前调用，但是必须在进入main之前的环境是单线程的情况下才能正常工作。

总之，个人建议最好不要在进入main函数之前获取Singleton对象。因为上文中的各种方法都用到了staitc member，而c++标准**只保证static member在进入main函数之前初始化，但是不同编译单元之间的static member的初始化顺序却是未定义的**， 所以如果在main之前就调用getInstance的话，就有可能出现实现Singleton的static member还没有初始化就被使用的情况。

如果万一要在main之前获取Singleton对象，并且进入main之前的环境是多线程环境，这种情形下，还能保证正常工作的写法只有C++ 11下的Meyers Singleton，或者如g++ 4.0及其后续版本这样的编译器提前支持内存模型情况下的C++ 03也是可以的。

## 参考文献
1. Scott Meyers. [Effective C++:55 Specific Ways to Improve Your Programs and Designs,3rd Edition. 电子工业出版社, 2011](https://book.douban.com/subject/5387403/)
2. Stanley B. Lippman. [深度探索C++对象模型. 电子工业出版社, 2012](https://book.douban.com/subject/10427315/)
3. Randal E.Bryant, David O’Hallaron. [深入理解计算机系统(第2版). 机械工业出版社, 2010](https://book.douban.com/subject/5333562/)
4. Scott Meyers. [C++ and the Perils of Double-Checked Locking. 2004](https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf)
5. W.Richard Stevens. [UNIX环境高级编程(第3版), 人民邮电出版社, 2014](https://book.douban.com/subject/25900403/)
