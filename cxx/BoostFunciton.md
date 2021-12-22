一种简单的boost::function实现。

要点：全特化，偏特化，函数对象，动态绑定。

注意：函数不存在偏特化，偏特化功能可以用重载实现。

func.h

```cpp
template<typename R, typename T1, typename T2>
class base
{
public:
	virtual ~base()
	{
	}
	virtual R operator()(T1, T2) = 0;
};


////////////////普通函数对象//////////////////
template<typename R, typename T1, typename T2>
class func:public base <R, T1, T2>
{
public:
	func(R (*ptr)(T1, T2))
		:ptr_(ptr)
	{
	}
	virtual R operator()(T1 a, T2 b)
	{
		return ptr_(a, b);
	}
private:
	R (*ptr_)(T1, T2);
};


/////////////////////成员函数对象/////////////////////
template<typename R, typename Class, typename T>
class member:public base<R, Class, T>
{

};

template<typename R, typename Class, typename T>
class member<R, Class*, T>:public base<R, Class*, T>
{
public:
	member(R (Class::*ptr)(T))
		:ptr_(ptr)
	{
	}
	virtual R operator()(Class* obj, T a)
	{
		return (obj->*ptr_)(a);
	}
private:
	R (Class::*ptr_)(T);
};


/////////////////////////////////////////
template<typename T, typename R, typename T1, typename T2>
class functor:public base<R, T1, T2>
{
public:
	functor(const T& obj)
		:obj_(obj)
	{
	}
	virtual R operator()(T1 a, T2 b)
	{
		return obj_(a, b);
	}
private:
	T obj_;
};


////////////////可用的function///////////////////
template<typename T>
class function
{
};

template<typename R, typename T1, typename T2>
class function<R (T1, T2)>
{
public:
	template<typename Class, typename _R, typename _T2>
	function(_R (Class::*ptr)(_T2))  // 将某个类的成员函数指针传递进来
		: ptr_(new member<R, T1, T2>(ptr))  // 用这个函数指针构造member对象，ptr是虚基类指针
	{
	}

	template<typename _R, typename _T1, typename _T2>
	function(_R (*ptr)(_T1, _T2))  // 普通函数指针
		: ptr_(new func<R, T1, T2>(ptr))
	{
	}

	template<typename T>
	function(const T& obj)
		: ptr_(new functor<T, R, T1, T2>(obj))
	{
	}

	~function()
	{
		delete ptr_;
	}

	virtual R operator()(T1 a, T2 b)
	{
		return ptr_->operator()(a, b);  // RTTI
	}

private:
	base<R, T1, T2>* ptr_;
};
```
main.c
```cpp
int main(int argc, char const *argv[])
{
	function<int (int, int)> foo(get);
	foo(10.1, 10.3);

	function<int (Point*, int)> bar(&Point::get);
	Point point;
	bar(&point, 30);
  return 0;
 }
 ```
