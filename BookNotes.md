面向对象编程有3个要素：**数据抽象、继承以及动态绑定**。
## One 代理类（Surrogate）

### 代码
```cpp
class Vehicle
{
public:
	virtual double weight() const = 0;
	virtual void start() = 0;
	virtual Vehicle* copy() const = 0;
}
class RoadVehicle:public Vehicle
{
	Vehicle* RoadVehicle::copy()
	{
		return new RoadVehicle(*this);
	}
}
class AutoVehicle:public RoadVehicle
{
	Vehicle* AutoVehicle::copy()
	{
		return new AutoVehicle(*this);
	}
}
class Aircraft:public Vehicle
{
}
class Helicopter:public Aircraft
{
}
///////////////////////////////////
class VehicleSurrogate
{
public:
	VehicleSurrogate();
	VehicleSurrogate(const Vehicle&);
	~VehicleSurrogate();
	VehicleSurrogate(const VehicleSurrogate&);
	VehicleSurrogate& operator=(const VehicleSurrogate&);
	//来自Vehicle类的操作
	double weight() const;
	void start();
private:
	Vehicle* vp;
}
VehicleSurrogate::VehicleSurrogate():vp(0)
{
}
VehicleSurrogate::VehicleSurrogate(const Vehicle& v):vp(v.copy())
{
}
VehicleSurrogate::~VehicleSurrogate()
{
	delete vp;
}
VehicleSurrogate::VehicleSurrogate(const VehicleSurrogate& v):vp(v.vp ? v.vp->copy() : 0)
{
}
VehicleSurrogate& VehicleSurrogate::operator=(const VehicleSurrogate& v)
{
	if(this != &v)
	{
		delete vp;
		vp = (v.vp ? v.vp->copy() : 0);
	}
	return this;
}
double VehicleSurrogate::weight const
{
	if(vp == 0)
	{
		throw "empty VehicleSurrogate.weight()"
	}
	return vp->weight();
}
void VehicleSurrogate::start()
{
	if(vp == 0)
	{
		throw "empty VehicleSurrogate.start()"
	}
	vp->start();
}
////////////////
int main()
{
	VehicleSurrogate parking_lot[1000];//Vehicle类是抽象类，不能实例化，通过代理类实现
	Automobile x;
	parking_lot[num_vehicles++] = x;
	//等价于
	parking_lot[num_vehicles++] = VehicleSurrogate(x);
}

```
### 思考

为了方便管理，我们构造一个容器或数组，包含类型不同但彼此相关的类。

假设剔除了所有纯虚函数，以下操作
```cpp
Vehicle* parking_lot[1000];
Automobile x;
parking_lot[num_vehicles++] = new Automobile(x);
```
会把x转换成一个Vehicle对象，同时会丢失所有在Vehicle类中没有的成员(slicing problem)，也增加了动态内存管理负担。

代理类将整个派生层次和动态内存管理压缩在一个类型中，**避免了显示处理内存分配，又能保持类在运行时动态绑定**。

## Two 句柄（Handle）

### 代码
```cpp
class Point
{
public:
	Point():xval(0),yval(0){}
	Point(int x, int y):xval(x), xval(y){}
	int x() const {return xval;}
	int y() const {return yval;}
	Point& x(int xv){xval = xv; return *this;}
	Point& y(int yv){yval = yv; return *this;}
private:
	int xval, yval;
}
///////////////////////
class Handle
{
public:
	Handle();
	Handle(int, int);
	Handle(const Point&);
	Handle(const Handle&);
	Handle& operate=(const Handle&);
	~Handle();
	int x() const;
	Handle& x(int);
	int y() const;
	Handle& y(int);
private:
	Upoint* up;
}
Handle::Handle():up(new UPoint)
{
}
Handle::Handle(int x, int y):up(new UPoint(x,y))
{
}
Handle::Handle(const Point& p):up(new UPoint(p))
{
}
Handle::~Handle()
{
	if(--up->u == 0)//Handle是UPoint的友元类，因此可以直接访问私有成员
	{
		delete up;
	}
}
Handle::Handle(const Handle& h):up(h.up)
{
	++up->u;
}
Handle& Handle::operate=(cosnt Handle& h)
{
	++h.up->u;
	if(--up->u == 0)
	{
		delete p;
	}
	up = h.up;
	return *this;
}
int Handle::x() const
{
	return up->p.x();
}
int Handle::y() const
{
	return up->p.y();
}
//////指针语义，不必复制UPoint对象/////
Handle& Handle::x(int x0)
{
	up->p.x(x0);
	return *this;
}
Handle& Handle::y(int y0)
{
	up->p.y(y0);
	return *this;
}
////值语义，必须保证改动的UPoint对象不能同时被任何其他Handle引用/////
///改为///
Handle& Handle::x(int x0)
{
	if(up->u != 1)//此种做法叫写时复制（CopyOnWrite），句柄类型库经常用到
	{
		--up->u;
		up = new UPoint(up->p);
	}
	up->p.x(x0);
	return *this;
}
Handle& Handle::y(int y0)
{
	if(up->u != 1)
	{
		--up->u;
		up = new UPoint(up->p);
	}
	up->p.y(y0);
	return *this;
}
///////////////////////
class UPoint
{
private:
	friend class Handle;
	Point p;
	int u;
	UPoint:u(1){}
	UPoint(int x, int y):p(x, y), u(1){}
	UPoint(const Point& p0):p(p0), u(1){}
}
```
### 思考

创建代理会复制所代理的对象，而句柄类（行为与指针类似，因此也称为智能指针）就**允许在保持代理的多态行为的同时，同时避免了复制这些对象**。

我们可针对是值语义还是指针语义来设计，如下

```cpp
int main()
{
	Handle h(3, 4);
	Handle h2 = h;	//复制Handle
	h2.x(5);		//修改Point
	int n = h.x();	//3（值语义）还是5（指针语义）
	return 0;
}
```

为了把句柄捆绑到类Point对象上，必须定义一个具有类型为Point的成员的新类UPoint，
当要将这样的句柄捆绑到继承自Point的（静态的）未知类对象时，局限性较大。
### 分离引用计数

### 代码

```cpp
class Point
{
public:
	Point():xval(0),yval(0){}
	Point(int x, int y):xval(x), xval(y){}
	int x() const {return xval;}
	int y() const {return yval;}
	Point& x(int xv){xval = xv; return *this;}
	Point& y(int yv){yval = yv; return *this;}
private:
	int xval, yval;
}
///////////////////////
class UseCount
{
public:
	UseCount();
	UseCount(const UseCount&);
	UseCount& operator=(const UseCount&);
	bool only();
	bool makeonly();
	bool reattach(const UseCount&);
	~UseCount();
private:
	UseCount& operator=(const UseCount& =);
	int* p;		//计数
}
UseCount::UseCount():p(new int(1))
{
}
UseCount::UseCount(const UseCount& u):p(u.p)
{
	++*p;
}
bool UseCount::only()
{
	return *p == 1;
}
bool UseCount::makeonly()
{
	if (*p == 1)
	{
		return false;
	}
	--*p;
	p = new int(1);
	return true;
}
bool UseCount::reattach(const UseCount& u)
{
	++*u.p;
	if(--*p == 0)
	{
		delete p;
		p = u.p;
		return true;
	}
	p = u.p;
	return false;
}

UseCount::~UseCount()
{
	if(--*p == 0)
	{
		delete p;
	}
}
//////////////////////
class Handle
{
public:
	Handle();
	Handle(int, int);
	Handle(const Point&);
	Handle(const Handle&);
	Handle& operate=(const Handle&);
	~Handle();
	int x() const;
	Handle& x(int);
	int y() const;
	Handle& y(int);
private:
	Point* p;	//使用Pint不仅能绑定Point对象，还能绑定到继承Point类的对象
	UseCount u;
}
Handle::Handle():p(new Point)
{
}
Handle::Handle(int x, int y):p(new Point(x,y))
{
}
Handle::Handle(const Point& p0):p(new Point(p0))
{
}
Handle::~Handle()
{
	if(u.only())
	{
		delete p;
	}
}
Handle::Handle(const Handle& h):u(h.u), p(h.p)
{
	++*u;
}
Handle& Handle::operate=(cosnt Handle& h)
{
	if(u.reattach(h.u))
	{
		delete p;
	}
	p = h.p;
	return *this;
}
int Handle::x() const
{
	return p->x();
}
Handle& Handle::x(int x0)
{
	if(u.makeonly())//判断point对象是否只绑定了一个句柄，值语义，指针语义删除这个if
	{
		p = new Point(*p);
	}
	p->x(x0);
	return *this;
}
int Handle::y() const
{
	return p->y();
}
Handle& Handle::y(int y0)
{
	if(u.makeonly())//判断point对象是否只绑定了一个句柄
	{
		p = new Point(*p);
	}
	p->y(y0);
	return *this;
}
```
### 思考

分离引用计数，顾名思义，把原来UPoint类分成了Point、UseCount两个类。

UseCount类可以在不了解其使用任何信息的情况下与之合为一体。这样一来，我们就可以把这个类当成句柄实现的一部分，与各种不同的数据结构协同工作。

## Tree 一个面向对象的程序范例

关于算术表达式的树。例如，表达式(-5)*(3+4)对应的树为：
我们希望这样构建：
```cpp
#include <iostream>
int main()
{
	Expr t = Expr("*", Expr("-", 5), Expr("+", 3, 4));
	cout<<t<<endl;
	t = Expr("*", t, t);
	cout<<t<<endl;
}
```
打印

((-5)*(3+4))

((-5)*(3+4))*((-5)*(3+4))

### 代码

```cpp
class Expr_node
{
	friend ostream& operator<<(ostream&, const Expr&);
	friend class Expr;
	int use;
protected:
	Expr_node():use(1);
	virtual void print(ostream&) const = 0;
	virtual ~Expr_node(){}
}
ostream& operator<<(ostream& o, const Expr_node& e)
{
	e.print(o);
	return o;
}
//////////////////////////////////
class Int_node:public Expr_node
{
	friend class Expr;
	int n;
	Int_node(int k):n(k){}
	void print(ostream& o) const
	{
		o<<n;
	}
}
////////////////////////////////
class Unary_node:public Expr_node
{
	friend class Expr;
	String op;
	Expr opnd;
	Unary_node(const String& a, Expr b):op(a), opnd(b){}
	void print(ostream& o) const
	{
		o<<"("<<op<<opnd<<")";
	}
}
\\\\\\\\\\\\\\\\\\
class Binary_node:public Expr_node
{
	friend class Expr;
	String op;
	Expr left;
	Expr right;
	Binary_node(const String& a, Expr b, Expr c):op(a), left(b), right(c){}
	void print(ostream& o) const
	{
		o<<"("<<left<<op<<right<<")";
	}
}
\\\\\\\\句柄类\\\\\\\\\\\\\\
class Expr
{
	friend ostream& operator<<(ostream&, const Expr&);
	Expr_node* p;
public:
	Expr(int);
	Expr(const String&, Expr);
	Expr(const String&, Expr, Expr);
	Expr(const Expr&);
	Expr& operator=(const Expr&);
	~Expr(){if(--p->use == 0)delete p;}
}
Expr::Expr(int n)
{
	p = new Int_node(n);
}
Expr::Expr(const String& op, Expr t)
{
	p = new Unary_node(op, t);
}
Expr::Expr(const String&, Expr, Expr)
{
	p = new Binary_node(op, left, right);
}
Expr::Expr(const Expr& t)
{
	p = t.p;
	++p->use;
}
Expr::Expr& operator=(const Expr& rhs)
{
	rhs.p->use++;
	if(--p->use == 0)
	{
		delete p;
	}
	p = rhs.p;
	return *this;
}

ostream& operator<<(ostream& o, const Expr& t)
{
	t.p->print(o);
	return o;
}
```

### 思考

## Four 字符图形拼接

### 代码
Picture.h
```cpp
#include <iostream>

using namespace std;

/////////////////////P_Node/////////////////////////////
class P_Node
{
public:
	friend class Picture;//Picture友元类可以访问P_Node类的所有成员
	friend Picture reframe(char, char, char);//友元函数可以访问P_Node类的所有成员
	virtual Picture reframe(char, char, char) = 0;//纯虚函数，抽象类不能实例化，子类必须实现
protected:
	P_Node();
	virtual ~P_Node();//必须申明为虚析构函数，防止内存泄漏
	virtual int height() const = 0;
	virtual int width() const = 0;
	virtual void display(ostream &, int, int) const = 0;
//private:
	int use;//私有成员不能被继承
};
/////////////////////Picture////////////////////////
class Picture
{
	friend ostream& operator<<(ostream&, const Picture&);
	friend Picture frame(const Picture&);
	friend Picture reframe(const Picture&, char, char, char);//灵活性
	friend Picture operator&(const Picture&, const Picture&);
	friend Picture operator|(const Picture&, const Picture&);
	friend class String_Pic;
	friend class Frame_Pic;
	friend class Hcat_Pic;
	friend class Vcat_Pic;
public:
	Picture();
	Picture(const char* const*, int);
	Picture(const Picture&);
	~Picture();
	Picture& operator=(const Picture&);
private:
	Picture(P_Node*);
	int height() const;
	int width() const;
	void display(ostream&, int, int) const;
	P_Node *p;
};
//////////////////String_Pic///////////////////
class String_Pic:public P_Node
{
	friend class Picture;
	String_Pic(const char* const*, int);
	~String_Pic();
	int height() const;
	int width() const;
	void display(ostream &, int, int) const;
	Picture reframe(char, char, char);
	char** data;
	int size;
};
////////////////Frame_Pic/////////////////////
class Frame_Pic:public P_Node
{
	friend Picture frame(const Picture&);
	Frame_Pic(const Picture&, char = '+', char = '|', char = '-');
	int height() const;
	int width() const;
	void display(ostream &, int, int) const;
	Picture reframe(char, char, char);
	Picture p;
	char corner;
	char sideborder;
	char topborder;
};
////////////Vcat_Pic///////////////
class Vcat_Pic:public P_Node
{
	friend Picture operator&(const Picture&, const Picture&);
	Vcat_Pic(const Picture&, const Picture&);
	int height() const;
	int width() const;
	void display(ostream &, int, int) const;
	Picture reframe(char, char, char);
	Picture top, bottom;
};
/////////////////Hcat_Pic/////////////////////
class Hcat_Pic:public P_Node
{
	friend Picture operator|(const Picture&, const Picture&);
	Hcat_Pic(const Picture&, const Picture&);
	int height() const;
	int width() const;
	void display(ostream &, int, int) const;
	Picture reframe(char, char, char);
	Picture left, right;
};
```
Picture.cpp
```cpp
#include "Picture.h"
////////////////////////P_Node/////////////////////
P_Node::P_Node():use(1)
{
	//
}
P_Node::~P_Node()
{
	//
}
//////////////////////////////////////////////
static void pad(ostream& os, int x, int y)//static作用：指对函数的作用域仅局限于本文件
{
	for (int i = x; i < y; i++)
	{
		os << " ";
	}
}
int max(int x, int y)
{
	return x > y ? x : y;
}
////////////////////////Picture///////////////////////////
Picture::Picture(P_Node* p_node):p(p_node)
{
	//
}
Picture::Picture(const char* const* str, int n):p(new String_Pic(str, n))
{
	//
}
Picture::Picture(const Picture& orig):p(orig.p)
{
	orig.p->use++;
}
Picture::~Picture()
{
	if (--p->use == 0)
	{
		delete p;
	}
}
Picture& Picture::operator =(const Picture& orig)
{
	orig.p->use++;
	if (--p->use == 0)
	{
		delete p;
	}
	p = orig.p;
	return *this;
}
int Picture::height() const
{
	return p->height();
}
int Picture::width() const
{
	return p->width();
}
void Picture::display(ostream & o, int x, int y) const
{
	p->display(o, x, y);
}
/////////////////////output///////////////////////
ostream& operator<<(ostream& os, const Picture& picture)
{
	int ht = picture.height();
	for (int i = 0; i < ht; i++) 
	{
		picture.display(os, i, 0);
		os<<endl;
	}
	return os;
}
////////////////////////String_Pic///////////////////////////
String_Pic::String_Pic(const char* const* p, int n):data(new char* [n]), size(n)
{
	for (int i = 0; i < n; i++)
	{
		data[i] = new char[strlen(p[i]) + 1];
		strcpy(data[i], p[i]);
	}
}
String_Pic::~String_Pic()
{
	for (int i = 0; i < size; i++)
	{
		delete[] data[i];
	}
	delete[] data;
}
int String_Pic::height() const
{
	return size;
}
int String_Pic::width() const
{
	int n = 0;
	for (int i = 0; i < size; i++)
	{
		n = max(n, strlen(data[i]));
	}
	return n;
}
void String_Pic::display(ostream & os, int row, int width) const
{
	int start = 0;
	if(row >= 0 && row < height())
	{
		os << data[row];
		start = strlen(data[row]);
	}
	pad(os, start, width);
}
Picture String_Pic::reframe(char, char, char)
{
	use ++;//use 如果是P_Node的私有成员，则不能被继承
 	return this;
}
////////////////////////Frame_Pic//////////////////////////
Frame_Pic::Frame_Pic(const Picture& pic, char c, char s, char t):p(pic), corner(c),sideborder(s),topborder(t)
{
	//
}
int Frame_Pic::height() const
{
	return p.height() + 2;
}
int Frame_Pic::width() const
{
	return p.width() + 2;
}
void Frame_Pic::display(ostream& os, int row, int wd) const
{
	if (row < 0 || row >= height())
	{
		pad(os, 0 , wd);
	}
	else
	{
		if (row == 0 || row == height() - 1)
		{
			os << corner;
			int i = p.width();
			while (--i >= 0)
			{
				os<<topborder;
			}
			os << corner;
		}
		else
		{
			os << sideborder;
			p.display(os, row - 1, p.width());
			os << sideborder;
		}
		pad(os, width(), wd);
	}
}
Picture Frame_Pic::reframe(char c, char s, char t)
{
	return new Frame_Pic(::reframe(p, c ,s ,t), c, s, t);//加域符号，调用全局的reframe函数而不是类内的reframe成员函数，防止编译时查找成员函数
}
/////////////////////////////////////////
Picture frame(const Picture& pic)
{
	return new Frame_Pic(pic);
}
Picture reframe(const Picture& pic, char c, char s, char t)
{
	return pic.p->reframe(c, s, t);
}
//////////////////Vcat_Pic////////////////////////
Vcat_Pic::Vcat_Pic(const Picture& t, const Picture& b):top(t), bottom(b)
{
	//
}
int Vcat_Pic::height() const
{
	return top.height() + bottom.height();
}
int Vcat_Pic::width() const
{
	return max(top.width(), bottom.width());
}
void Vcat_Pic::display(ostream & os, int row, int wd) const
{
	if (row >= 0 && row < top.height())
	{
		top.display(os, row, wd);
	}
	else if (row < top.height() + bottom.height())
	{
		bottom.display(os, row - top.height(), wd);
	}
	else
	{
		pad(os, 0, wd);
	}
}
Picture Vcat_Pic::reframe(char c, char s, char t)
{
	return new Vcat_Pic(::reframe(top, c, s, t), ::reframe(bottom, c, s, t));//加域符号，调用全局的reframe函数而不是类内的reframe成员函数
}
////////////////////Hcat_Pic////////////////////////////
Hcat_Pic::Hcat_Pic(const Picture& l, const Picture& r):left(l),right(r)
{
	//
}
int Hcat_Pic::height() const
{
	return max(left.height(), right.height());
}
int Hcat_Pic::width() const
{
	return left.width() + right.width();
}
void Hcat_Pic::display(ostream & os, int row, int wd) const
{
	left.display(os, row, left.width());
	right.display(os, row, right.width());
	pad(os, width(), wd);
}
Picture Hcat_Pic::reframe(char c, char s, char t)
{
	return new Hcat_Pic(::reframe(left, c, s, t), ::reframe(right, c, s, t));
}
//////////////////////////////////////////////////
Picture operator&(const Picture& t, const Picture& b)
{
	return new Vcat_Pic(t, b);
}

Picture operator|(const Picture& l, const Picture& r)
{
	return new Hcat_Pic(l, r);
}
```

### 思考

## Five 设计容器类

Array.h
### 代码
```cpp
#include <iostream>
#include <iomanip>
using namespace std;
template<class T> class Array;
template<class T> class Pointer;
//////////////////Array_data///////////////////////
template<class T> class Array_data
{
	friend class Array<T>;
	friend class Pointer<T>;
	Array_data(unsigned size = 0):data(new T[size]), sz(size), use(1)
	{

	}
	~Array_data()
	{
		delete [] data;
	}
	const T& operator[] (unsigned n) const
	{
		if (n >= sz)
		{
			throw "Array subscript out of range";
		}
		return data[n];
	}
	T& operator[] (unsigned n)
	{
		if (n >= sz)
		{
			throw "Array subscript out of range";
		}
		return data[n];
	}
	void resize(unsigned n)
	{
		if (n == sz)
		{
			return;
		}
		T* odata = data;
		data = new T[n];
		copy(odata, sz > n ? n : sz);
		delete [] odata;
		sz = n;
	}
	void copy(T* arr, unsigned n)
	{
		for (int i = 0; i < n; i++)
		{
			data[i] = arr[i];
		}
	}
	void reserve(unsigned new_sz)
	{
		if (new_sz >= data->sz)
		{
			data->grow(new_sz);
		}
	}
	void grow(unsigned new_sz)
	{
		unsigned nsz = sz;
		if (nsz == 0)
		{
			nsz = 1;
		}
		while (nsz <= new_sz)
		{
			nsz *= 1.5;
		}
		resize(nsz);
	}
	//没有实现，不允许复制操作
	Array_data(const Array_data&);
	Array_data& operator=(const Array_data&);
	T* data;
	unsigned sz;
	int use;
};
//////////////////Array///////////////////////
template<class T> class Array
{
	friend class Pointer<T>;
public:
	Array(unsigned size):data(new Array_data<T>(size))
	{
	}
	Array(const Array& a):data(new Array_data<T>(a.data->sz))
	{
		data->copy(a.data->data, a.data->sz);
	}
	Array& operator=(const Array& a)
	{
		if (this != &a)
		{
			data->clone(*a.data, a.data->data);
			return *this;
		}
	}
	void clone(const Array_data& a, unsigned n)
	{
		delete data;
		data = new T[sz = a.sz];
		copy(a.data, sz);
	}
	~Array()
	{
		if (--data->use == 0)
		{
			delete data;
		}
	}
	const T& operator[](unsigned n) const
	{
		return (*data)[n];
	}
	T& operator[](unsigned n)
	{
		return (*data)[n];
	}
	void resize(unsigned n)
	{
		data->resize(n);
	}
private:
	Array_data<T>* data;
};
//////////////////Ptr_to_const///////////////////////
template<class T> class Ptr_to_const
{
public:
	Ptr_to_const(const Array<T>& a, unsigned n = 0):ap(a.data),sub(n)
	{
		++ap->use;
	}
	Ptr_to_const():ap(0),sub(0)
	{

	}
	Ptr_to_const(const Ptr_to_const<T>& p):ap(p.ap),sub(p.sub)
	{
		if (ap)
		{
			++ap->use;
		}
	}
	~Ptr_to_const() 
	{
		if (ap && --ap->use == 0)
		{
			delete ap;
		}
	}
	Ptr_to_const& operator=(const Ptr_to_const<T>& p)
	{
		if (p.ap)
		{
			++p.ap->use;
		}
		if (ap && --ap->use == 0)
		{
			delete ap;
		}
		ap = p.ap;
		sub = p.sub;
		return *this;
	}
	//返回const T&，而不是T&
	const T& operator*() const
	{
		if (ap == 0)
		{
			throw "* of unbound Ptr_to_const";
		}
		return (*ap)[sub];
	}
private:
	Array_data<T>* ap;
	unsigned sub;
};
//////////////////Pointer///////////////////////
template<class T> class Pointer: public Ptr_to_const<T>
{
	friend int operator-(const Ptr_to_const<T>& op1, const Ptr_to_const<T>& op2);
	friend Pointer<T> operator+(const Pointer<T>& p, int n);
	friend Pointer<T> operator+(int n, const Pointer<T>& p);
public:
	Pointer(Array<T>& a, unsigned n = 0):Ptr_to_const<T>(a, n)
	{
	}
	Pointer()
	{
	}
	T& operator*() const
	{
		if (ap == 0)
		{
			throw "* of unbound Ptr_to_const";
		}
		return (*ap)[sub];
	}
	Pointer& operator++()//前置
	{
		++sub;
		return *this;
	}
	Pointer& operator++(int)//后置
	{
		Pointer ret = *this;
		++sub;
		return ret;
	}
	Pointer& operator--()
	{
		--sub;
		return *this;
	}
	Pointer& operator--(int)
	{
		Pointer ret = *this;
		--sub;
		return ret;
	}
	Pointer& operator+=(int n)
	{
		sub += n;
		return *this;
	}
	Pointer& operator-=(int n)
	{
		sub -= n;
		return *this;
	}
};

template <class T> int operator-(const Ptr_to_const<T>& op1, const Ptr_to_const<T>& op2)//指针相减
{
	return (int)op1.sub - (int)op2.sub;
}

template <class T> Pointer<T> operator+(const Pointer<T>& p, int n)//指针增加
{
	Pointer<T> ret = p;
	return ret += n;
}
template <class T> Pointer<T> operator+(int n, const Pointer<T>& p)
{
	Pointer<T> ret = p;
	return ret += n;
}
```
### 思考

## Six 函数对象

Comp.h
### 代码
```cpp
#include <iostream>

using namespace std;

template<class X, class Y>
class Comp_base
{
public:
	virtual Y operator()(X) const = 0;
	virtual Comp_base* clone() const = 0;
	virtual ~Comp_base(){}
};

template<class F, class G, class X, class Y>
class Comp:public Comp_base<X, Y>
{
public:
	Comp(F f0, G g0):f(f0), g(g0)
	{

	}
	Y operator()(X x) const
	{
		return f(g(x));
	}
	Comp_base<X, Y>* clone() const
	{
		return new Comp(*this);
	}
private:
	F f;
	G g;
};

template<class X, class Y>
class Compsition//代理类
{
public:
	template<class F, class G> Compsition(F, G);//针对Compsition(X, Y)每一个变量，都定义了构造函数Compsition(F, G)
	Compsition(const Compsition&);
	Compsition& operator=(const Compsition&);
	~Compsition();
	Y operator() (X) const;
//private:
	Comp_base<X, Y>* p;
};

template<class X, class Y>
template<class F, class G>
Compsition<X, Y>::Compsition(F f, G g):p(new Comp<F, G, X, Y>(f, g))
{

}

template<class X, class Y>
Compsition<X, Y>::~Compsition()
{
	delete p;
}

template<class X, class Y>
Compsition<X, Y>::Compsition(const Compsition& c):p(c.p->clone())
{

}

template<class X, class Y>
Compsition<X, Y>& Compsition<X, Y>::operator=(const Compsition& c)
{
	if (this != &c)
	{
		delete p;
		p = c.p->clone();
	}
	return *this;
}

template<class X, class Y>
Y Compsition<X, Y>::operator() (X x) const
{
	return (*p)(x);
}
```
main.cpp
```cpp
#include "Comp.h"

extern int f(int);
extern int g(int);
extern int h(int);
int f(int)
{
	return 0;
}
int g(int)
{
	return 0;
}
int h(int)
{
	return 0;
}

int main()
{
	Compsition<int, int> fg(f, g);
	Compsition<int, int> gh(g, h);
}
```
### 思考
假设需要使用以下语句
```cpp
int (*h)(int) = compose(f, g);
```
根据函数指针，假设有以下代码

```cpp
int (*compose(int f(int), int g(int))) (int x)
{
	int result(int n){return f(g(n));}
	return result;
}
```

compose有两个原因不能成功：第一，**C++不支持嵌套函数**。第二，result需要在块作用域内访问f和g，所以没有简便的方法可以绕开限制。

假设C++允许支持嵌套函数，实际上也不会成功。
```cpp
int (*compose(int f(int), int g(int))) (int x)
{
	int (*fp)(int) = f;
	int (*gp)(int) = g;
	int result(int n){return fp(gp(n));}
	return result;
}
```
因为fp和gp是compose的局部变量，一旦compose返回，它们就消失了。

## Seven 在簇中分配对象

### 代码

### 思考

## Eight 应用器、操纵器和函数对象
有整整一群这样的函数，它们**获得并且返回对某个类型的引用，而且以某种方式操纵该类型的对象**，这样的函数为操纵器。

操纵器本身很笨拙，试想它用来写几个值，并在每次写完之后清空文件：
```cpp
cout<<x;
flush(cout);
cout<<y;
flush(cout);
cout<<z;
flush(cout);
```
- **第一种解决方案**

定义一个FLUSH的任意类型。这个新类只用于重载operator<<，因此不需要成员：
```cpp
class FLUSH{}
FLUSH f;
ostream& operator<<(ostream& o, FLUSH f)
{
	return flush(o);
}
```
那么就可以直接“写”这个变量了：
```cpp
cout<<x<<f<<y<<f<<z<<f;
```
就是一个对ostream& operator<<(ostream&, FLUSH)的调用。

- **第二种解决方案**

第一种方案有2个缺点：第一，对于每个操纵器，必须笨拙地定义一个类似FLUSH的新伪类型，如果有很多操纵器，就需要定义许多新类型。
第二，除了定义这些类型外，我们还必须定义一系列类似f的伪对象(dummy object)。

使用一个小技巧就可以消除这些负担：

```cpp
ostream& operator<<(ostream& o, ostream& (*func) (ostream&))
{
	return (*func)(o);
}
```
我们称这个操作符为应用器。**它的右操作数是一个操纵器，左操作数是被操纵的对象。应用器对它的左参数运用操纵器。**

- **多个参数**

比如打印十六进制的n，我们希望能够这样写：

```cpp
cout<<to_hex(n);
```
这个额外的参数将问题复杂化了。

### 代码

```cpp
class long_fn_obj
{
public:
	long_fn_obj (ostream& (*f) (ostream&, long), long v):func(f),val(v)
	{

	}
	ostream& operator()(ostream& o) const//第三步
	{
		return (*func)(o, val);
	}
private:
	ostream& (*func)(ostream&, long);
	long val;
};
ostream& operator<<(ostream& ofile, const long_fn_obj& im)//第二步
{
	return im(ofile);
}
ostream& hexconv(ostream& ofile, long n)//第四步
{
	return ofile<<to_hex(n);
}
long_fn_obj hexconv(long n)
{
	return long_fn_obj( (ostream& (*) (ostream&, long)) hexconv, n);//第一步，hexconv是重载函数，需要强转
}
int main()
{
	cout<<hexconv(m)<<" "<<hexconv(n);//现在可以这样写
	return 0;
}
```

- **简化**

对于每个操纵器，我们都需要有一个函数对象模型，以及一个针对该操纵器的各种参数组合而定制的应用器。但是不同
的函数对象和应用器的定义都很相似，这就意味着我们可以用模板来简化。

```cpp
template<class stype, class vtype>
class fcn_obj
{
public:
	fcn_obj(stype& (*f) (stype&, vtype), vtype v):func(f),val(v){}
	stype& operator() (stype& s) const
	{
		return (*func) (s, val);
	}
private:
	stype& (*func) (stype&, vtype);
	vtype val;
};

template<class stype, class vtype>
stype& operator<<(stype& ofile, const fcn_obj<stype, vtype>& im)
{
	return im(ofile);
}

fcn_obj<ostream, long>
hexconv(long n)
{
	ostream (*my_hex) (ostream&, long) = hexconv;//想要此句有效，必须在之前有指定的hexconv的声明
	return fun_obj<ostream, long>(my_hex, n);
}
cout<<hexconv(m)<<" "<<hexconv(n);
```

### 思考

将z<<f;转换为f(z)的调用。对于z能够拥有的每种类型（istream，ostream等）来说，只有一种类型可以用于f，
因此我们只需要定义一个用于z的可能类型的应用器。

然后我们考虑具有一个额外参数的操纵器，将z<<f(x);与f(z,x)对应起来。我们需要为z和x的每个可能的类型对定义一个
应用器<<和一个函数对象。

显然我们可以扩展z<<f(a1, a2, ..., an);与f(z, a1, a2, ..., an)对应。

写cin>>noecho>>password>>echo;

比写

noecho(cin);

cin>>password;

echo(cin);

或者

echo(noecho(cin)>>pasword);简单!

## 参考文献
1. Scott Meyers. [Effective C++:55 Specific Ways to Improve Your Programs and Designs,3rd Edition. 电子工业出版社, 2011](https://book.douban.com/subject/5387403/)
2. Andrew Koenig / Barbara Moo. [C++沉思录. 人民邮电出版社, 2008](https://book.douban.com/subject/2970056/)
