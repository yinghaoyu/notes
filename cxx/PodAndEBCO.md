### POD（Plain Old Data，普通旧数据）类型是从 C++11 开始引入的概念

- Plain 代表一个对象是一个普通类型
- Old 代表一个对象可以与 C 兼容

### 普通类型
当类或结构体同时满足如下几个条件时是普通类型：

1. 没有虚函数或虚基类；
2. 由C++编译器提供默认的特殊成员函数（默认的构造函数、拷贝构造函数、移动构造函数、赋值运算符、移动赋值运算符和析构函数）；
3. 数据成员同样需要满足条件（1）和（2）。

### 标准布局类型
当类或结构体同时满足如下几个条件时是标准布局类型：

1. 没有虚函数或虚基类；
2. 所有非静态数据成员都具有相同的访问说明符；
3. 在继承体系中最多只有一个类中有非静态数据成员；
4. **子类中的第一个非静态成员的类型与其基类不同；**
此规则是因为 C++ 允许优化不包含成员基类而产生的。
在 C++ 标准中，如果基类没有任何数据成员，基类应不占用空间，为了体现这一点，C++ 标准允许派生类的第一个成员与基类共享同一地址空间。
但是如果派生类的第一个非静态成员的类型和基类相同，由于 **C++ 标准要求相同类型的对象的地址必须不同**，编译器就会为基类分派一个字节的地址空间。

### POD 类型
一个对象既是普通类型（Trivial Type）又是标准布局类型（Standard-layout Type）那么这个对象就是POD类型。
为什么我们需要 POD 类型满足这些条件呢，POD 类型在源码层级的操作上兼容于 ANSI C。POD 对象与 C 语言中的对象具有一些共同的特性，包括初始化、复制、内存布局与寻址：

1. 可以使用字节赋值，比如用 memset、memcpy 对 POD 类型进行赋值操作；
2. 对 C 内存布局兼容，POD 类型的数据可以使用 C 函数进行操作且总是安全的；
3. 保证了静态初始化的安全有效，静态初始化可以提高性能，如将 POD 类型对象放入 BSS 段默认初始化为 0。

### EBCO(empty base optimization-EBO 或 empty base class opimization-EBCO, 空白基类最优化）

在空基类被继承后由于没有任何数据成员，所以子类优化掉基类所占的1 byte。**EBO并不是c++标准所规定必须的。**

```cpp

#include<iostream>
 
using namespace std;
 
class Base
{};
 
class Derived1:public Base
{};
 
class Component 
{
  int x;
  Base t;      
};
 
class Derived2:public Base
{
  int x;
};
 
int main()
{
   Base t;
   printf("size of Base: %d\n",sizeof(Base));//1，被编译器安插进去的一个char，这样可以使得这个class的两个objects在内存中配置独一无二的地址 
   printf("size of t: %d\n",sizeof(t));        //1
   printf("size of Derived1: %d\n",sizeof(Derived1));  //1
   printf("size of Component: %d\n",sizeof(Component));  //8，字节对齐 
   printf("size of Derived2: %d\n",sizeof(Derived2));  //4，空白基类最优化
   return 0;
}
```
