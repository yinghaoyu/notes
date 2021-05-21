## 虚函数表

**虚函数（Virtual Function）** 是通过一张 **虚函数表（Virtual Table）** 来实现的。简称为 **V-Table**。在这个表中，主是要一个类的虚函数的地址表，这张表解决了继承、覆盖的问题，保证其容真实反应实际的函数。当我们用父类的指针来操作一个子类的时候，它就像一个地图一样，**指明了实际所应该调用的函数**。

> 注意，虚函数表是针对类而言的，所有类对象共享一张虚函数表，这张表是在编译时形成的。

假设有个名为FatherA的类：

``` cpp
class FatherA
{
public:
	virtual void funA();
	virtual void funB();
	virtual void funC();
	void fun();
	virtual void ~FatherA();
};
```

则它的虚函数表如下所示：

<div align="center">

![20200909_1](/images/20200909_1.png)

</div>

- **单重继承（无虚函数覆盖）**

假设有以下继承关系：

<div align="center">

![20200909_2](/images/20200909_2.png)

</div>

ChildA没有重载任何FatherA的函数，那么ChildA的虚函数表如下：

<div align="center">

![20200909_3](/images/20200909_3.png)

</div>

> 注意，虚函数按照其声明的顺序置于表中，父类的虚函数在子类虚函数之前。

- **单重继承（有虚函数覆盖）**

假设有以下继承关系：

<div align="center">

![20200909_4](/images/20200909_4.png)

</div>

ChildA重载了FatherA的funA函数，那么ChildA的虚函数表如下：

<div align="center">

![20200909_5](/images/20200909_5.png)

</div>

> 注意，覆盖的子类虚函数ChildA::funA被置于虚表中原来父类虚函数FatherA::funA的位置，没有被覆盖的函数依旧。

- **多重继承（无虚函数覆盖）**

假设有以下继承关系：

<div align="center">

![20200909_6](/images/20200909_6.png)

</div>

ChildB没有重载任何FatherA、FatherB、FatherC的函数，那么ChildB的虚函数表如下：

<div align="center">

![20200909_7](/images/20200909_7.png)

</div>

> 注意，每个父类都有自己的虚函数表。子类的未覆盖的虚函数被放到了第一个继承的父类的表中（这里的第一个父类是按照声明顺序来判断的）。

- **多重继承（有虚函数覆盖）**

假设有以下继承关系：

<div align="center">

![20200909_8](/images/20200909_8.png)

</div>

ChildB覆盖了FatherA、FatherB、FatherC的funA、funB、析构函数，那么ChildB的虚函数表如下：

<div align="center">

![20200909_9](/images/20200909_9.png)

</div>

这里引入3个问题：

1. 为什么父类的析构函数必须是虚函数？

	- 当我们动态申请一个子类对象时，使用基类指针指向该对象，如果不用虚函数，**子类的析构函数不能得到调用**，也就是为了在释放基类指针时可以释放掉子类的空间，防止内存泄漏。

2. 为什么C++默认的析构函数不是虚函数？

	- 因为虚函数需要额外的虚函数表和虚表指针，**占用额外的内存**。而对于不会被继承的类来说，其析构函数如果是虚函数，就会浪费内存。

> 对于虚析构函数而言，每个析构函数结束时会自动（隐含地）调上父类的析构函数（子类先析构，父类再析构）。父类析构不经过虚表，直接调用父类的析构函数（效果相当于this->Father::~Father()）。虽然Child的虚表里面没有父类析构函数的地址，但是只要Father::~Father有定义，就不妨碍直接调用。

3. 为什么要使用父类指针和引用实现多态，而不能使用对象？

	- 因为参数如果不是指针或者引用，会进行拷贝。子类对象赋值给父类对象，切片（slicing problem）把子类中包含的父类成员变量的值拷贝过去，但是子类的虚表不会给拷贝过去，则函数中这个父类对象的虚表是父类的，所以无法实现多态。

而指针或者引用是直接指向子类对象，不会进行拷贝赋值，这样虚函数表是子类的虚函数表，故能实现多态。

- **父类类型的指针指向子类实例**

假设有以下继承关系：

``` cpp
class FatherA
{
public:
	FatherA();
	
	virtual void funA();
	virtual void funB();
	virtual void funC();
	
	void fun();
	
	virtual void ~FatherA();
};
class FatherB
{
public:
	FatherB();
	
	virtual void funA();
	virtual void funB();
	virtual void funC();

	void fun();

	virtual ~FatherB();
};

class FatherC
{
public:
	FatherC();
	
	virtual void funA();
	virtual void funB();
	virtual void funC();

	void fun();

	virtual ~FatherC();
};

class ChildB : public FatherA, public FatherB, public FatherC
{
public:
	ChildB();

	void funA();
	void funB();

	virtual void funD();

	void fun();

	virtual ~ChildB();
};

typedef void(*fun)(void);

int main()
{
	FatherB* obj = new ChildB();
	fun p = (fun)*( (int*) *(int*)(obj) + 2);
	p();					//输出fatherB funC
	return 0;
}
```
则以p为起始地址的虚函数表如下：

<div align="center">

![20200909_10](/images/20200909_10.png)

</div>

> 注意，则以p为起始地址的虚函数表中，FatherA的虚函数表地址仿佛已经被“截断”了，只剩下了FatherB和FatherC的虚函数表地址。

但任然可以访问FatherA的虚函数表

``` cpp
int main()
{
	FatherB* obj = new ChildB();
	fun p = (fun)*( (int*) *(int*)(obj) + 2);
	p();					//输出fatherB funC
	fun q = (fun)*( (int*) *((int*)(obj) - 1) + 2);
	q();					//输出fatherA funC
	return 0;
}
```

## 安全性隐患

- 通过父类型的指针访问子类自己的虚函数

``` cpp
typedef void(*fun)(void);

int main()
{
	fatherA* obj3 = new childB();
	obj3->funD();							//编译器认为非法
	
	fun p24 = (fun)*( (int*) *(int*)(obj3) + 4);
	p24();									//输出childB funD	
}
```

> 注意，任何妄图使用父类指针想调用子类中的未覆盖父类的成员函数的行为都会被编译器视为非法，所以，这样的程序根本无法编译通过。但在运行时，我们可以通过指针的方式访问虚函数表来达到违反C++语义的行为。

- 访问non-public的虚函数

> 如果父类的虚函数是private或是protected的，但这些非public的虚函数同样会存在于虚函数表中，所以，我们同样可以使用访问虚函数表的方式来访问这些non-public的虚函数，

## 补充例子
```c
class A
{
public:
	A()
	{
		cout<<"A"<<endl;
	}
	virtual ~A()
	{
		cout<<"~A"<<endl;
	}
};
class B:public A
{
public:
	B()
	{
		cout<<"B"<<endl;
	}
	 ~B()
	{
		cout<<"~B"<<endl;
	}
};

void funX()//说明是以B的虚表访问析构函数
{
	B b;//依次输出 A, B
	b->B::~B();//依次输出 ~B, ~A
}
void funY()//说明是以B的虚表访问析构函数，动态多态
{
	B b;//依次输出 A, B
	b->~B();////依次输出 ~B, ~A
}

void funZ()//说明是以A的虚表访问析构函数
{
	B b;////依次输出A, B
	b->A::~A();//依次输出 ~A
}

void funA()//两次析构并没有改变虚表A
{
	B b;//依次输出 A, B
	b->A::~A();//依次输出  ~A
	b->A::~A();//依次输出  ~A
}

void funB()//两次析构并没有改变虚表B
{
	B b;//依次输出 A, B
	b->B::~B();//依次输出 ~B, ~A
	b->B::~B();//依次输出 ~B, ~A
}

void funC()//与funY对比，说明前一个析构，改变了b->~B()的析构方式（没有用子类的虚表访问~B），与funA、funB对比，说明没有改变A、B虚表
{
	B b;//依次输出 A, B
	b->B::~B();//依次输出 ~B, ~A
	b->~B();//依次输出 ~A
}

void funD()//与funY对比，说明前一个析构，改变了b->~B()的析构方式（没有用子类的虚表访问~B），与funA、funB对比，说明没有改变A、B虚表
{
	B b;//依次输出 A, B
	b->A::~A();//依次输出 ~A
	b->~B();//依次输出 ~A
}
```

本博客所有原创文章均采用[CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)许可协议。

您可以复制共享、演绎创作，但不得用于商业目的，转载请注明原创作者 **raining888** 。
