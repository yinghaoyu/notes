这是笔者的学习C语言的小磕碰，大多数都是细节问题。

1. 
| 字符 | ASCII码 |
|:-|:-|
| `0~9` | `48~57` |
| `A~Z` | `65~90` |
| `a~z` |` 97~122` |
| `\0` | `0`、`NULL` |
> 小写字母比大写字母的ASCII码值大`32`。

2. 少用但却巧用的赋值方法
	``` c
	int m =(n>6)&&(n<8);
	```

3. 尽量用double型，float型的精度约为6~7位，double型有15~16位，例如：
	```c
	float f1=8765.4;
	float f2=8765.4;
	double d1=8765.4;
	double d2=8765.4;
	f1*f2=76832244.007969;
	d1*d2=76832237.1600000;
	```

	> 由上可见，float在整数部分已经不正确了。

4. π可以精确定义为
	``` c
	const double pi = acos(-1.0)
	```
	> 因为`cos(pi)=-1`（引用头文件math.h）。

5. round(double x)函数可以将double型变量4舍5入（引用头文件math.h）。

6. 浮点数四舍五入最简单的写法：
	``` c
	float n;
	scanf("%f",&n);
	if (n>=0)
		printf("%d\n",n+0.5);
	else
		printf("%d\n",n-0.5);
	```
	> 经实验，有些编译器会直接根据32位float直接输出一个很大的int型数。

7. 声明数组大小必须是整数常量，不能是变量。

8. 如果数组大小较大（大概为10<sup>6</sup>级别），需要定义在主函数外面，否则会使程序异常退出，原因是函数内部申请的局部变量来自系统栈，允许申请的空间较小；而函数外部申请的全局变量来自静态存储区，允许申请的空间较大。
	例如：
	``` c
	int a[1000000]；
	int main()
	{
		return 0;
	}
	```

9. memset给数组赋值：
	``` c
	int a[5];
	memset(a, 0, sizeof(a));//0的补码全为0
	memset(a, -1, sizeof(a));//-1的补码全为1
	memset(a, 1, sizeof(a));//1的补码为0000 0001
	```
	> 注意第3个memset，int为4个字节，memset为每个字节填`0000 0001`，所以最后为32位为：`0000 0001 0000 0001 0000 0001 0000 0001`，最终a的值为16843009。

10. printf中%c能识别空格和换行并将其输入，%s识别空格作为字符串结尾;getchar也能识别空格和换行；gets识别换行符\n作为输入结束，因此，scanf完一个整数后，如果要用gets，需要先用getchar接收整数后的换行符。如果不是用scanf的%s格式或gets输入字符，一定要在每个字符串后加入'\0'，否则printf和puts输出字符串会出现乱码，例如：
	``` c
	char str[15];
	int i;
	for (i = 0; i< 3;i++)
	{
		str[i] = getchar();
	}
	puts(str);
	return 0;
	```

11. 可以理解为sscanf=string+scanf，sprintf=string+printf，sscanf(str, "%d", &n);将str中内容以%d格式写到n中，sprintf(str, "%d", n)同理;

12. 指针是一个unsigned类型的整数。int* p =&a;其中int *是指针变量类型，p才是变量名，因此&a是赋值给p而不是*p的。

13. 引用是产生变量的别名，因此常量不可使用引用。引用=变量的别名≠指针。

14. 浮点数比较一般不用`==`，因为在机器中，`3.14`可能存储为`3.140000000001`或者`3.1399999999999`，`==`（C和C++中）是完全相同才会判定为`true`，可以引入极小数eps = 10<sup>-8</sup>，相减与eqs比较即可。

15. scanf函数的返回值为其成功读入的参数个数，失败返回-1，一般不会失败，只有在读取文件尾，无法读取才会失败，使用EOF（End Of File）来代表-1。因此可写:
	``` c
	while(scanf("%d", &a) != EOF)
	{
	}
	```
	> 一般可以通过<Ctrl+Z>组合键触发EOF。

16. 查找数据为int型，如果二分查找上界超过int型数据范围的一半，那么当预查询元素在序列较靠后的位置时，语句`mid=(left+right)/2`中的`left+right`就可能超过int而导致溢出，此时一般用`mid=left+(right-left)/2`等价语句代替，可以避免溢出。

17. 条件`if(b%2==1)`，可以使用`if(b&1)`代替，位操作比求余快。

18. 下面递归算法中，else中先算出了单个`binaryPow(a, b/2, m)`，是正确的。如果直接返回`binaryPow(a, b/2, m)*binaryPow(a, b/2, m)%m`，会调用两个`binaryPow`函数，导致复杂度变为O(2<sup>log(b)</sup>) = O(b)。
	``` c
	LL binaryPow(LL a, LL b, LL m)
	{
		if(b==0)
		{
			return1;
		}
		if(b%2==1)
		{
			return a*binaryPow(a, b-1, m)%m;
		}
		else
		{
			LL mul = binaryPow(a, b/2, m);
			return mul*mul%m;
		}
	}
	```
	
	>例如，求`binaryPow(8)`时，会变成`binaryPow(4)*binaryPow(4)`，这两个`binaryPow(4)`都会变成`binaryPow(2)*binaryPow(2)`，每个`binaryPow(2)`又会变成`binaryPow(1)*binaryPow(1)`，最后需要求`8`次`binaryPow(1)`。

19. 求a<sup>13</sup>，13的二进制为`b=1101`，13=2<sup>3</sup> + 2<sup>2</sup> + 2<sup>0</sup> = 8 + 4 + 1，a<sup>13</sup> = a<sup>8</sup> * a<sup>4</sup> * a<sup>1</sup>，可以对b进行移位，末位为1，乘a，进行自身平方操作。a<sup>n</sup>用快速幂，时间复杂度降低到log<sub>2</sub>n。

20. 有些编译器重载>会编译出错，<则不会。

21. 结构体数据较为庞大时，建议用引用来提高效率，例如：形参const int &p。

22. free函数和delete操作符一样，释放指针变量p所指向的内存空间，并将指针变量p指向空地址NULL，指针变量p本身并没消失。

23. 一般容易发生的编程失误，在if后面多了语句分隔符;，这种情况眼神不好，难以发现，如下：
	``` c
	if(条件);
	{
		执行语句;
	}
	```

24. 在a.cpp里申明的对象，作为外部引用的话，需要在a.h里加上extern，例如:

	``` c
	//a.h
	extern int a;
	```
	``` c
	//a.cpp
	int a;
	```

25. 类与函数指针的结合使用
	``` cpp
	Class Classname
	{
	public:
		void fun1(int);
		void fun2(int);
	};

	typedef void (ClassName::*ptr)(int); 

	ClassName ObjName;

	Function(ptr ptr1, ptr ptr2, int arg)
	{
		(ObjName->*ptr1)(arg);
	}

	int main()
	{
		int a;
		Function(&ClassName::fun1, &ClassName::fun2, a);
		return 0;
	}
	```

26. 程序优化，例如：
	``` c
	int* lengths = (int*)malloc(sizeof(int) * (k + 1));
	for (int i = 0; i <= k; ++i)
	{
		lengths[i] = shorter * (k - i) + longer * i;
		//1、可改成lengths[i] = lengths[i-1] + longer - shorter; 
		//2、同时可定义常量difference = longer - shorter；放置循环外，减少计算。
		//3、可改成temp = temp + difference; 访问数组比变量慢。
	}
	```

27. `x & -x = x & (~x + 1)`，一个数取负数等于先按位求反再加1。当一个偶数与它的负值相与时， 结果是能整除这个偶数的最大的2的幂, 即： `m = n & -n` , 则 `n % m = 0`, 且 m = 2<sup>k</sup>。如果是x是奇数， 那`x & -x` 的结果一定是`1`。应用：[树状数组](/docs/datastruct/bit)。

28. 构造函数，拷贝构造函数及重载运算符
	``` cpp
	class A
	{
	private:
		int *a;
		int b;
	public:
		A(const A& other)
		{
			a = new int;//深拷贝
			b = other.b;
		}
		A& operate=(const A& other)//重载=号
		{
			A(other);
			return *this;
		}
	}；
	```
29. 以下程序的输出结果是？
	``` c
	int main()
	{
		int a=1,b=2,m=0,n=0,k;
		 
		k=(n=b<a)&&(m=a) ;
		printf("%d,%d\n",k,m);
		 
		return 0;
	}
	```
	> 结果：0,0。注意，先执行 n=b<a 部分，其中，关系运算符优先级高于赋值运算符，所以先算 b<a，得到false，C++中，false和0可以相互转换，所以n=b<a的结果是n=0；然后，赋值运算的结果将作为括号内表达式的结果，即(n=b<a)&&(m=a)转换成(0)&&(m=a)，而0又相当于false，使&&后面的括号“短路”(||也有相同“短路”效果)，（m=a）不运算，m值还是0最后，&&的结果是false，即k=0；

30. 输入参数要有加const修饰的习惯，要有异常处理。

31. 如果编写一个标准strcpy函数的总分值为10，下面给出几个不同得分的答案：

	- 2分答案如下：

	``` c
	void strcpy(char* strDest, char* strSrc)
	{
		while((*strDest++ = *strSrc++) != '\0');
	}
	```

	- 4分答案如下：

	``` c
	void strcpy(char* strDest, const char* strSrc)		//将源字符串加const，表明其为输入参数。
	{
		while((*strDest++ = *strSrc++) != '\0');
	}
	```

	- 6分答案如下：

	``` c
	void strcpy(char* strDest, const char* strSrc) 
	{
		assert( (strDest != NULL) && (strSrc != NULL) ); //对源地址和目的地址加非0断言
		while( (*strDest++ = * strSrc++) != ‘\0’ );
	}
	```

	- 8分答案如下：
	``` c
	char* strcpy(char* strDest, const char* strSrc) 
	{
		assert( (strDest != NULL) && (strSrc != NULL) );
		char *address = strDest; 
		while( (*strDest++ = * strSrc++) != ‘\0’ ); 
		return address;									//为了实现链式操作，将目的地址返回
	}
	```

	- 10分答案如下：

	``` c
	void* memcpy(void* strDest,const void* strSrc,unsigned int count)
	{
		 assert(strDest != NULL);
		 assert(strSrc != NULL);
		 void* ret = strDest;
		 if (strDest <= strSrc || (char*)strDest >= ((char*)strSrc + count))//源地址和目的地址不重叠，低字节向高字节拷贝
		 {
			 while(count--)
			 {
				 *(char*)strDest = *(char*)strSrc;
				 strDest = (char*)strDest + 1;
				 strSrc = (char*)strSrc + 1;
			 }
		 }
		 else						//源地址和目的地址重叠，高字节向低字节拷贝
		 { 
			 strDest = (char*)strDest + count - 1;
			 strSrc = (char*)strSrc + count - 1; 
			 while(count--) 
			 {
				 *(char*)strDest = *(char*)strSrc;
				 strDest = (char*)strDest - 1;
				 strSrc = (char*)strSrc - 1;
			 }
		}
		return ret;
	}

	char* my_strcpy(char *strDest,const char *strSrc)
	{
		assert(strDest != NULL);
		assert(strSrc != NULL);
		char *ret = strDest;
		memcpy(strDest,strSrc,strlen(strSrc)+1);
		return ret;
	}
	```

	> 注意，要考虑内存重叠情况，例如char str[10] = "abc"; strcpy(str+1,str);当str+1指向'\0'时，str指向'c'，'\0'字符会被取代，导致后续访存越界。实际上，C语言中标准函数也确实考虑到此种情况。

32. 下面代码会出现什么问题？
	``` c
	char *GetMemory( void )
	{ 
		char p[] = "hello world"; 
		return p; 
	}
	void Test( void )
	{ 
		char *str = NULL; 
		str = GetMemory(); 
		printf( str ); 
	}
	```
	> char p[]="hello world";相当于char p[12]，strcpy(p," hello world" )；p是一个数组名，属于局部变量，存储在栈中， " hello world" 存储在文字存储区，数组p中存储的是 " hello world" 的一个副本，当函数结束，p被回收，副本也消失了(确切的说p指向的栈存储区被取消标记，可能随时被系统修改)，而函数返回的p指向的内容也变得不确定，文字存储区的 " hello world" 未改变。可以这样修改: ①char* p= " hello world" ; return p; 这里p直接指向文字存储区的 " hello world" ，函数按值返回p存储的地址，所以有效。 ②static char p[]= " hello world" ; return p; static指出数组p为静态数组，函数结束也不会释放，所以有效。 

33. 指针传值问题：
	``` cpp
	#include <iostream>

	using namespace std;

	//传值调用
	void GetMemory( char **p )
	{
		*p = (char *) malloc( 100 );
	}
	//引用调用
	void GetMemory_1(char *&p)
	{
		p = (char *) malloc (100);
	}

	int main()
	{
		char *str = NULL;
		char *str1 = NULL;
		GetMemory( &str );
		GetMemory_1( str1 );
		strcpy( str, "hello world" );
		strcpy( str1, "hello world1" );
		cout<<str<<endl;
		cout<<str1<<endl;
		free(str);
		free(str1);
		return 0;
	}
	```
34. 写一个“标准”宏MIN，这个宏输入两个参数并返回较小的一个。另外，当你写下面的代码时会发生什么事？  

	``` c
	least = MIN(*p++, b);
	```
	``` c
	#define MIN(A,B) ((A) <= (B) ? (A) : (B)) 		//正确
	#define MIN(A,B) (A) <= (B) ? (A) : (B)			//错误, 宏的作用是替换，整个宏不加括弧，会有歧义，3*MIN(A,B)会被解析成3*(A) <= (B) ? (A) : (B)
	#define MIN(A,B) (A <= B ? A : B )				//错误
	#define MIN(A,B) ((A) <= (B) ? (A) : (B)); 		//错误
	```
	
	宏定义#define MIN(A,B) ((A) <= (B) ? (A) : (B))对MIN(\*p++, b)的作用结果是：
	
	``` c
	((*p++) <= (b) ? (*p++) : (b)) 					//指针p会作2次++自增操作，副作用
	```
	
	>注意，宏的作用是替换，应小心歧义与副作用。

35. 
	``` c
	#ifndef __INCvxWorksh
	#define __INCvxWorksh 
	#ifdef __cplusplus
	extern "C" {
	#endif 
	/*...*/ 
	#ifdef __cplusplus
	}
	#endif 
	#endif /* __INCvxWorksh */
	```
	- 头文件中的编译宏作用是防止被重复引用。
	
	- 作为一种面向对象的语言，C++支持函数重载，而过程式语言C则不支持。函数被C++编译后在symbol库中的名字与C语言的不同。例如，假设某个函数的原型为：
	
	void foo(int x, int y);
	
	该函数被C编译器编译后在symbol库中的名字为_foo，而C++编译器则会产生像_foo_int_int之类的名字。_foo_int_int这样的名字包含了函数名和函数参数数量及类型信息，C++就是靠这种机制来实现函数重载的。
	
	为了实现C和C++的混合编程，C++提供了C连接交换指定符号extern "C"来解决名字匹配问题，函数声明前加上extern "C"后，则编译器就会按照C语言的方式将该函数编译为_foo，这样C语言中就可以调用C++的函数了。
	
36. 如何用一段程序判断机器的大小端方式？
	``` c
	int main()
	{
		union w
		{
			int a;
			char b;
		}c;
		c.a = 1;
		if(c.b == 1)
		{
			//little_endian
		}
		else
		{
			//big_endian
		}
		
	}
	```
	> union 联合体是共用内存区域，也就是说int 和 char一起公用4byte.并且union一定是从低地址开始存放，所以char b对应最低内存区域。如果是大端存储，int的1存在最高位，其他全为0，小端存储时1存在最低位，所以只要判断b是否为0即可。

37. 关于static关键字
	
	- 全局静态变量
		
		- 在全局变量前加static，在静态存储区中，从定义处开始，到文件结尾，其他文件不可见。
	
	- 局部静态变量
		
		- 在局部变量前加static，在静态存储区中，从定义处作用域开始，到作用域结尾。
		
	- 静态函数
		
		- 在函数前加static，默认情况都是extern的，但只在声明文件中可见，不能被其他文件调用，不会与其他文件中同名函数冲突。

	- 类静态成员
		
		- 与该类的对象无关，供该类的所有对象共用，没有破坏隐藏的原则，保证了安全性。
	
	- 类的静态函数
	
		- 同类静态成员，在静态成员函数的实现中不能直接引用类中说明的非静态成员，可以引用类中说明的静态成员（这点非常重要）。如果静态成员函数中要引用非静态成员时，可通过对象来引用。从中可看出，调用静态成员函数使用如下格式：<类名>::<静态成员函数名>(<参数表>);
38. 关于const关键字

	- 普通类型变量
		
		- 在普通类型变量前加const，表示该变量为常量，其值不允许修改。
	
	- 指针类型变量
	
		- `const int* p`，其中 `*p` 是不能改变的，即内容不可变。
		
		- `int const* p`，这样的写法不科学，`int*` 应该当成一个整体，不过它的含义与上面的相同。
		
		- `int* const p`，其中 `p` 是不能改变的，即指针指向的地址不可变。
	
	- 参数传递
	
		- 防止形参被修改。
		
			> C++中，自定义类型的参数传递，需要临时对象复制参数，对于临时对象的构造，需要调用构造函数，比较浪费时间，因此我们采取 const 外加引用传递的方法。
	
	- 函数返回值
	
		- 修饰自定义类型的作为返回值，此时返回的值不能作为左值使用，既不能被赋值，也不能被修改。
	
	- 类成员函数
	
		- 防止成员函数修改被调用对象的值，如果我们不想修改一个调用对象的值，所有的成员函数都应当声明为 const 成员函数。
		
			> 注意，const 关键字不能与 static 关键字同时使用，因为 static 关键字修饰静态成员函数，静态成员函数不含有 this 指针，即不能实例化，const 成员函数必须具体到某一实例。

39. 指针和引用的区别

	- 指针有自己的一块空间，而引用只是一个别名；
	
	- 使用sizeof看一个指针的大小是4（32bit），而引用则是被引用对象的大小；
	
	- 指针可以被初始化为NULL，而引用必须被初始化且必须是一个已有对象 的引用；
	
	- 作为参数传递时，指针需要被解引用才可以对对象进行操作，而直接对引用的修改都会改变引用所指向的对象；
	
	- 可以有const指针，但是没有const引用；
	
	- 指针在使用中可以指向其它对象，但是引用只能是一个对象的引用，不能被改变；
	
	- 指针可以有多级指针（如\*\*p），而引用至于一级；
	
	- 指针和引用使用++运算符的意义不一样；
	
	- 如果返回动态内存分配的对象或者内存，必须使用指针，引用可能引起内存泄露。
	
40. 对一个变量根据条件进行不同类型定义

	``` c
	# if condition
		int
	# else
		char
	# endif
		a;
	```
	
41. 
	``` c
	#pragma warning(push)是保存当前的编译器警告状态；
	#pragma warning(pop)是恢复原先的警告状态。
	例如：
	#pragma warning( push )
	#pragma warning( disable : 4705 )
	#pragma warning( disable : 4706 )
	#pragma warning( disable : 4707 )
	// Some code
	#pragma warning( pop ) 
	```
	> 注意，这样在编译Some code部分的代码时，4705、4706、4707三个警告将不会出现。
	
42. swap函数有另一种写法：
	```c
	void swap(int *x, int *y)
	{
		if(x == y) return;//地址相同就不做操作。
		*y = *x ^ *y;
		*x = *x ^ *y;
		*y = *x ^ *y;
	}
	```
	> 注意，这种方式并没有性能上的优势，仅仅是一个智力游戏。这个函数存在一个缺陷，int指针x和y不能指向相同的空间，否则得到的值会变成0。

43. 二进制转无符号数：B2U<sub>4</sub>([1011]) = 1 * 2 <sup>3</sup> + 0 * 2 <sup>2</sup> + 1 * 2 <sup>1</sup> + 1 * 2 <sup>0</sup> = 11

	二进制转补码：B2T<sub>4</sub>([1011]) = - 1 * 2 <sup>3</sup> + 0 * 2 <sup>2</sup> + 1 * 2 <sup>1</sup> + 1 * 2 <sup>0</sup> = -5

44. 条件传送替换条件分支。

45. C没有真正的字符串概念，尽管以下是合法的：
	```c
	char hello[] = 'hello';//6个元素声明了字符数组'\0'为结尾
	//相似写法
	char *hello;
	hello = "hello";
	```
	但是这样写就不对
	```c
	char hello[5];
	hello = "hello";//此写法错误，因为c没有复制数组的内建方法
	```
46. 动态分配二维数组
	```c
	int** num = (int**)malloc(row_size*sizeof(int*));//sizeof(int*)为指针大小，返回值为int**
	for(int i = 0; i < row_size; i++)
	{
		num[i] = (int*)malloc(column_size*sizeof(int));//sizeof为(int)为实际元素大小，返回值为int*
		for(int j = 0; j < column_size; j++)
		{
			num[i][j] = value;
		}
	}
	```

本博客所有原创文章均采用[CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)许可协议。

您可以复制共享、演绎创作，但不得用于商业目的，转载请注明原创作者 **raining888** 。
