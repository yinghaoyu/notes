## C语言中的指针

- **关于const**

``` c
int num;
const int* p = &num;
int const* p = &num;// 这样的写法不科学，int* 应该当成一个整体，不过它的含义与上面的相同。
int* const p = &num;
```

> 对于第一种写法，`*p` 是不能改变的，即内容不可变；对于第三种写法，地址`p`是不能被改变的，即地址不可变。

- **指针数组**

``` c
int* a[10];
```

`[]`的优先级比`*`要高。a 先与`[]`结合，构成一个数组的定义，数组名为`a`，`int*`修饰的是数组的内容，即数组的每个元素。它包含`10`个指向`int`类型数据的指针，即指针数组。

sizeof(int* [10]) = 40

<div align="center">

![ptrArray](/images/ptrArray.png)

</div>

- **数组指针**

``` c
int (*b)[10];
```

`()`的优先级比`[]`高，`*`号和`b`构成一个指针的定义，指针变量名为`b`，`int`修饰的是数组的内容，即数组的每个元素。数组在这里并没有名字，是个匿名数组。`b`是一个指针，它指向一个包含`10`个`int`类型数据的数组，即数组指针。

sizeof(int (*)[10]) = 4

<div align="center">

![arrayPtr](/images/arrayPtr.png)

</div>

也许这样写更容易理解 
``` c
int(*)[10] b;
```

`int (*)[10]`是指针类型，`b`是指针变量。这样看起来的确不错，不过就是样子有些别扭。其实数组指针的原型确实就是这样子的，只不过为了方便与好看把指针变量`b`前移了。你私下完全可以这么理解这点。虽然编译器不这么想。

- **函数指针**

定义：函数指针是指向函数的指针变量。函数指针本身首先是一个指针变量，该指针变量指向一个具体的函数。

用途：调用函数和做函数的参数，比如回调函数。

示例：

``` c
char* fun(char * p)  {…}       // 函数fun

char* (*pf)(char * p);             // 函数指针pf

int (*f)(int*);		//函数指针

int *f(int*);		//没有括号会解读成，一个函数原型，返回值为int*

pf = fun;                        // 函数指针pf指向函数fun

pf(p);                        // 通过函数指针pf调用函数fun
```

- **Case 1**

``` c
int a[] = {1,2,3,4};
printf("%d\n",sizeof(a));				//16，a指数组首元素的地址
printf("%d\n",sizeof(a+0));				//4，数组退化为指针，数组作函数的形参也会发生退化
printf("%d\n",sizeof(*a));				//4
printf("%d\n",sizeof(a+1));				//4，a指数组首元素的地址，+1步长为4
printf("%d\n",sizeof(a[1]));			//4
printf("%d\n",sizeof(&a));				//4，&a整个指数组的地址，&a与a值一样，含义不一样
printf("%d\n",sizeof(*&a));				//16
printf("%d\n",sizeof(&a+1));			//4，&a整个指数组的地址,+1步长为16
printf("%d\n",sizeof(&a[0]));			//4
printf("%d\n",sizeof(&a[0]+1));			//4
```

根据C99 6.3.2章节，第3条。

> Except when it is the operand of the sizeof operator or the unary & operator, or is a string literal used to initialize an array, an expression that has type ‘‘array of type’’ is converted to an expression with type ‘‘pointer to type’’ that points to the initial element of the array object and is not an lvalue. If the array object has register storage class, the behavior is undeﬁned.

有3种情况不会发生array decay：1. sizeof操作符 2. 取址& 3. 字符串常量初始化的数组。

- **Case 2**

``` c
char arr[] = {'a','b','c','d','e','f'};
printf("%d\n", sizeof(arr));			//6，注意不是7，区别于Case 3
printf("%d\n", sizeof(arr+0));			//4
printf("%d\n", sizeof(*arr));			//1
printf("%d\n", sizeof(arr[1]));			//1
printf("%d\n", sizeof(&arr));			//4
printf("%d\n", sizeof(&arr+1));			//4
printf("%d\n", sizeof(&arr[0]+1));		//4

printf("%d\n", strlen(arr));			//任意整数x >= 6
printf("%d\n", strlen(arr+0));			//任意整数x >= 6
printf("%d\n", strlen(*arr));			//error，参数无意义
printf("%d\n", strlen(arr[1]));			//error，参数无意义
printf("%d\n", strlen(&arr));			//error，参数无意义
printf("%d\n", strlen(&arr+1));			//error，参数无意义
printf("%d\n", strlen(&arr[0]+1));		//任意整数x >= 5
```

> 注意，此种声明方式，arr数组不会补`'\0'`结束符，因此strlen(arr)的大小为遇到第一个`'\0'`字符后的长度。

- **Case 3**

``` c
char arr[] = "abcdef";
printf("%d\n", sizeof(arr));			//7，注意不是6，区别于Case 2
printf("%d\n", sizeof(arr+0));			//4
printf("%d\n", sizeof(*arr));			//1
printf("%d\n", sizeof(arr[1]));			//1
printf("%d\n", sizeof(&arr));			//4
printf("%d\n", sizeof(&arr+1));			//4
printf("%d\n", sizeof(&arr[0]+1));		//4

printf("%d\n", strlen(arr));			//6
printf("%d\n", strlen(arr+0));			//6
printf("%d\n", strlen(*arr));			//error，参数无意义
printf("%d\n", strlen(arr[1]));			//error，参数无意义
printf("%d\n", strlen(&arr));			//error，参数无意义
printf("%d\n", strlen(&arr+1));			//error，参数无意义
printf("%d\n", strlen(&arr[0]+1));		//5
```

> 注意，strlen不计算'\0'，sizeof计算分配的空间。

- **Case 4**

``` c
char *p = "abcdef";
printf("%d\n", sizeof(p));				//4，指针长度
printf("%d\n", sizeof(p+1));			//4，指针长度
printf("%d\n", sizeof(*p));				//1，单个字符长
printf("%d\n", sizeof(p[0]));			//1，单个字符长
printf("%d\n", sizeof(&p));				//4，指针长度
printf("%d\n", sizeof(&p+1));			//4，指针长度
printf("%d\n", sizeof(&p[0]+1));		//4，指针长度

printf("%d\n", strlen(p));				//6
printf("%d\n", strlen(p+1));			//5
printf("%d\n", strlen(*p));				//error，参数无意义
printf("%d\n", strlen(p[0]));			//error，参数无意义
printf("%d\n", strlen(&p));				//error，参数无意义
printf("%d\n", strlen(&p+1));			//error，参数无意义
printf("%d\n", strlen(&p[0]+1));		//5
```

> 指针为什么也可以用`[]`运算符？对于指针`int* p = "abc";` ，`p[1]`等价于 *(p + 1)，这是因为数组很多时候可以隐式转换成指针。

- **Case 5**

``` c
int a[3][4] = {0};
printf("%d\n",sizeof(a));				//48，指二维数组的地址长度,即int**[]的长度
printf("%d\n",sizeof(a[0][0]));			//4，a[0][0]元素长度，即int的长度
printf("%d\n",sizeof(a[0]));			//16，第0行行地址，即int*[]的长度
printf("%d\n",sizeof(a[0]+1));			//4，a[0][1]元素地址，即int*的长度
printf("%d\n",sizeof(*(a[0]+1)));		//4，a[0][1]元素长度，即int的长度
printf("%d\n",sizeof(a+1));				//4，第1行行地址，即int**的长度
printf("%d\n",sizeof(*(a+1)));			//16，第1行行地址，即int*[]的长度
printf("%d\n",sizeof(*(a+1) + 0));		//4，a[1][0]元素地址，即int*的长度
printf("%d\n",sizeof(&a[0]+1));			//4，第1行行地址，即int**的长度
printf("%d\n",sizeof(*(&a[0]+1)));		//16，第1行行地址，即int*的长度
printf("%d\n",sizeof(*a));				//16，指二维数组首行行地址即int*[]的长度
printf("%d\n",sizeof(*a+1));			//4，*a执行加法操作后，还是指针，不过数组的数组已退化为指针，即为int*的长度
printf("%d\n",sizeof(a[3]));			//16，超出数组a的行地址，已越界
```

> 注意，二维数组其实就是一个包含数组的数组。`a`其实是一个指针的指针，对指针进行加减操作，不会改变类型。`a+1`的类型为`int* [3]`,而`*(a+1)`的类型为`int*`。

- **Case 6**

``` c
int main()
{
	int a[5] = { 1, 2, 3, 4, 5 };
	int *ptr = (int *)(&a + 1);
	printf( "%d, %d\n", *(a + 1), *(ptr - 1));		//2，5
	return 0;
}
```

> 注意，`&a`类型为`int**`，`+1`偏移为整个数组的长度20，转成`int*`后，`-1`偏移为`int*`长度4。

- **Case 7**

``` c
struct Test
{
	int Num;
	char *pcName;
	short sDate;
	char cha[2];
	short sBa[4];
}*p;
//假设p 的值为0x100000。 如下表表达式的值分别为多少？
int main()
{
    printf("%p\n", p + 0x1);					//0x100014
    printf("%p\n", (unsigned long)p + 0x1);		//0x100001
    printf("%p\n", (unsigned int*)p + 0x1);		//0x100004
    return 0;
}
```

- **Case 8**

``` c
int main()
{
    int a[4] = { 1, 2, 3, 4 };					
    int* ptr1 = (int*)(&a + 1);				
    int* ptr2 = (int*)((int)a + 1);			
    printf( "%x,%x\n", ptr1[-1], *ptr2);		//4，20000
    return 0;
}
```

此时`ptr2`指向了`a[0]`的第二个字节。我们以`x86`平台为例（小端序），此时其内部数据分布是这样的：

<div align="center">

![littleEndian](/images/littleEndian.png)

</div>

因此将会打印出`0200 0000`。

> 注意，小端方式，低地址存低字节，高地址存高字节；大端方式，低地址存高字节，高地址存低字节。

- **Case 9**

``` c
int main()
{
    int a[3][2] = { (0, 1), (2, 3), (4, 5) };
    int *p;
    p = a[0];   
    printf( "%d", p[0]);					//1

    return 0;
}
```

> 注意，是`()`，而不是`{}`，这种赋值方式，把`(0, 1)`中的`1`赋值给了`a[0][0]`，把`(2, 3)`中的`3`赋值给了`a[0][1]`，把`(4, 5)`中的`5`赋值给了`a[0][2]`。

- **Case 10**

``` c
int main(){
    int a[5][5];
    int(*p)[4];
    p = (int (*)[4]) a;
    printf( "%p,%d\n", &p[4][2] - &a[4][2], &p[4][2] - &a[4][2]);	//FFFFFFFC，-4

    return 0;
}
```

假设`a`的首地址为`0x100000`，则数组`a`所有元素`a[i][j]`对应的地址为

`0x1000000` `0x1000004` `0x1000008` `0x100000c` `0x1000010`

`0x1000014` `0x1000018` `0x100001c` `0x1000020` `0x1000024`

`0x1000028` `0x100002c` `0x1000030` `0x1000034` `0x1000038`

`0x100003c` `0x1000040` `0x1000044` `0x1000048` `0x100004c`

`0x1000050` `0x1000054` `0x1000058` `0x100005c` `0x1000060`

数组指针`p`指向的数组所有元素对应的内容`p[i]`为
	
`0x1000000` `0x1000010` `0x1000020` `0x1000030`

`&p[4][2]`为`0x1000048`，`&a[4][2]`为`0x1000058`

> 注意，`p`指向的是一个内容为`4`个`int`的数组，偏移量按照新申明的类型计算。`p+4`的偏移量是`4*sizeof(4*int)`，因此`&p[4]`为`0x1000040`，`&p[4]+2`的偏移量为`2*sizeof(int)`，因此`&p[4][2]`为`0x1000048`。`&p[4][2]`和`&a[4][2]`相减是指针偏移量。

- **Case 11**

``` c
int main()
{
    int aa[2][5] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int *ptr1 = (int *)(&aa + 1);							//偏移量为1个aa数组
    int *ptr2 = (int *)(*(aa + 1));							//第二行地址，int(*)[5]
    printf( "%d,%d", *(ptr1 - 1), *(ptr2 - 1));				//10，5

    return 0;
}
```

> 注意，偏移量应按照转换后的类型`int*`计算。

- **Case 12**

``` c
int main()
{
    char *a[] = {"work","at","alibaba"};
    char**pa = a;
    pa++;
    printf("%s\n", *pa);									//at

    return 0;
} 
```

- **Case 13**

``` c
int main()
{
    char* c[] = {"ENTER","NEW","POINT","FIRST"};		//指针数组，每个元素存放类型为char*，c为char**
    char** cp[] = {c+3,c+2,c+1,c};						//指针数组，每个元素存放类型为char**，cp为char***
    char*** cpp = cp;									
    printf("%s\n", **++cpp);							//POINT，cpp是cp数组的起始地址，指向c+3，cpp为char***
														//++cpp使cpp指向了c+2，因此对c+2两次取值就得到了POINT的起始地址，**++cpp为char*
    printf("%s\n", *--*++cpp+3);						//ER，++cpp使cpp指向了c+1，对cpp取值操作，*cpp是“NEW“起始的地址
														//--*cpp指向了”ENTER“的起始地址，*--*cpp+3指向了“E”

    printf("%s\n", *cpp[-2]+3);							//ST，cpp指向c+1，cpp[-2]指向“FIRST”的首地址，*cpp[-2]指向“F”，+3偏移
    printf("%s\n", cpp[-1][-1]+1);						//EW，cpp指向c+1，cpp[-1]指向“POINT”的首地址，cpp[-1][-1]指向“NEW”，+1偏移

    return 0;
}
```

> 注意，单目运算符从右向左依次运算。

## C++中的对象指针

```cpp
class A
{
public:
    int a;
};

class B
{
public:
    double b;
};

class D:public A, public B
{
};

int main()
{
    D d;
    D* d = &d;
    B* b = &d;
    printf("b = %p\n", b);  // 0019FF0C
    printf("d = %p\n", d);  // 0019FF04
    if (b == d)
    {
        printf("b == d\n");  //实际上，b和d的值不相等，却输出b==d
    }
    else
    {
        printf("b != d\n");
    }
    return 0;
}
```
1. 当2个指针的静态类型以及所指对象的类型都**属于同一个继承层次结构**，其中一个指针类型是所指对象的静态类型，指针的比较，实际上比较的是两个指针是否指向同一个对象。
若2个指针指向同一个对象，被编译器决议为相等。编译器会在比较的时候隐式加上offset:

```cpp
if((b - sizeof(A) == d)
```

offset是由c++对象的内存模型决定的，详细请参考《深度探索C++对象模型》（候捷）。

若2个指针指向不同的对象，就被决议为不相等，并且比较的是指针保存的地址的值的大小。

```cpp
int main()
{
    D d1;
    D d2;
    D* d = &d1;
    B* b = &d2;
    printf("%p\n", d);
    printf("%p\n", b);
    if (d < b)
    {
        printf("d < b\n");  // 栈地址向上增长，输出d < b
    }
    else if (d == b)
    {
        printf("d == b\n");
    }
    else
    {
        printf("d > b\n");  // 栈地址向下增长，输出d > b
    }
    return 0;
}
```

2. 当2个指针的静态类型**不属于同一个继承层次结构**，但是2个指针都指向同一个对象的时候，该比较是违法行为，会报编译错误

```cpp
int main()
{
    D d;
    D* d = &d;
    int* b = reinterpret_cast<int*>(&d);
    printf("b = %p\n", b);
    printf("d = %p\n", d);
    if (b == d)  // error 没有从D *到int *的转换
    {
        printf("b == d\n");
    }
    else
    {
        printf("b != d\n");
    }
    return 0;
}
```

3. 当2个指针的**静态类型以及所指对象类型都属于同一个继承层次结构**，但是2个指针的静态类型都不是所指对象的类型时，该比较是违法行为，编译器会报编译期错误：

```cpp
int main()
{
    D d;
    B* b = &d;
    A* a = &d;
    printf("b = %p\n", b);
    printf("a = %p\n", a);
    if (b == a)  // error 没有从BaseA *到BaseB *的转换
    {
        printf("b == a\n");
    }
    else
    {
        printf("b != a\n");
    }
    return 0;
}
```

本博客所有原创文章均采用[CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)许可协议。

您可以复制共享、演绎创作，但不得用于商业目的，转载请注明原创作者 **raining888** 。
