根据《深入理解计算机系统（第3版）》总结的学习笔记。

- **背景**

编写函数tsub_ok的代码，参数是x和y，执行的运算是x-y，如果计算x-y不产生溢出，函数就返回1.假设你写的代码如下：

``` c
int tadd_ok(int x, int y) {
    int sum = x + y;
    int neg_over = x < 0 && y < 0 && sum >= 0;//两个数都为负数，但和却为正数，说明负溢出
    int pos_over = x >= 0 && y >= 0 && sum < 0;//两个数都为正数，但和却为负数，说明正溢出

    return !neg_over && !pos_over;//有一个取反后为0，就会返回0
}

int tsub_ok(int x, int y)
{
    return tadd_ok(x,-y);
}
```

此函数能检测到两个数相加，若发生溢出，则返回0，否则返回1。

- **错误原因**

如果y的值为INT_MIN（-2<sup>31</sup>），那么-y则应为2<sup>31</sup>，但实际效果是-y还是-2<sup>31</sup>。这样就会造成程序错误。

因为这里发生了**正溢出**。可以从两种角度进行解释：

1. 补码截断。

**初始时**y为INT_MIN（-2<sup>31</sup>）。

十六进制为0x8000 0000。

**取反后**-y应为2<sup>31</sup>

十六进制为0x0 8000 0000。需要多出1位来表示符号位。

但最终变量是int型的，所以必须截断。

所以，**最终**-y的值为-2<sup>31</sup>。

2. 溢出时运算。

一般来说，正溢出时，则可以认为运算结果得到了第33位二进制的权值2<sup>32</sup>。所以就应该是：

初始时，y为-2<sup>31</sup>。

取反后，-y为2<sup>31</sup>。

因为正溢出，所以减去2<sup>32</sup> ，-y为2<sup>31</sup> - 2<sup>32</sup>，最终为-2<sup>31</sup> 。

- **正确版本**
``` c
#include <stdio.h>

int tadd_ok(int x, int y)
{
    int sum = x + y;
    int neg_over = x < 0 && y < 0 && sum >= 0;//两个数都为负数，但和却为正数，说明负溢出
    int pos_over = x >= 0 && y >= 0 && sum < 0;//两个数都为正数，但和却为负数，说明正溢出

    return !neg_over && !pos_over;//有一个取反后为0，就会返回0
}

int tsub_ok(int x, int y)
{
    if(y == INT_MIN)//只要y为INT_MIN，就直接返回0
    	return 0;
    return tadd_ok(x, -y);
}
```

本博客所有原创文章均采用[CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)许可协议。

您可以复制共享、演绎创作，但不得用于商业目的，转载请注明原创作者 **raining888** 。
