下图是实现条件传送和条件跳转的思想。其中x，y的值是不可预测的，条件传送会计算x-y和y-x的值，而条件跳转只会计算x-y或者y-x的值，各有优势。

<div align="center">

![20201209](/images/20201209.png)

</div>

- 条件跳转

<div align="center">

![jump](/images/jump.png)

</div>

- 条件跳转O1优化

<div align="center">

![jumpO1](/images/jumpO1.png)

</div>

- 条件传送

<div align="center">

![comve](/images/cmove.png)

</div>

- 条件传送O1优化

<div align="center">

![comveO1](/images/cmoveO1.png)

</div>

- **条件传送与条件跳转**
	- 条件传送更符合现代处理器的特性。处理器使用流水线（pipeling）来获得高性能，通过**重叠连续指令**的步骤来获得高性能，例如，在取一条指令的同时，执行他前面一条指令的运算。
	这就需要**事先确定**执行指令的序列，这样才能保证流水线充满待执行的指令。
	当机器遇到条件跳转（即分支）时，只有当分支条件求值完成之后，才能决定分支往哪边走，处理器采用**分支预测**逻辑来猜测每条指令是否会执行，错误预测一个跳转，处理器会**丢掉**为该跳转指令后所有指令已做的工作，然后再开始用**从正确位置处起始的指令去填充流水线**。
	错误的预测，浪费大概15~30个时钟周期，导致程序性能严重下降。
	
	- 条件传送，处理器无需预测测试的结果就可以执行条件传送。
	
	- 使用条件传送也不总是会提高代码的效率。例如，then-expr或者else-expr的求值需要大量的计算，那么当相对应的条件不满足时，这些工作都白费了。

- **分支预测错误处罚**

	- 假设预测错误的概率是p，如果没有预测错误，执行代码的时间是T<sub>ok</sub>，而预测错误处罚是T<sub>mp</sub>。
	那么，作为p的一个函数，执行代码的平均时间是T<sub>avg</sub>(p) = (1 - p)T<sub>ok</sub> + p(T<sub>ok</sub> + T<sub>mp</sub>) = T<sub>ok</sub> + T<sub>mp</sub>。
	如果已知T<sub>ok</sub>和T<sub>ran</sub>（当p = 0.5时的平均时间），要确定T<sub>mp</sub>。将参数代入等式，我们有T<sub>ran</sub> = T<sub>avg</sub>（0.5） = T<sub>ok</sub> + 0.5T<sub>mp</sub>，所以有T<sub>mp</sub> = 2(T<sub>ran</sub> - T<sub>ok</sub>)。
	因此，对于T<sub>ok</sub> = 8和T<sub>ran</sub> = 17.5，我们有T<sub>ok</sub> = 19。

## 参考文献
1. Randal E.Bryant, David O’Hallaron. [深入理解计算机系统(第2版). 机械工业出版社, 2010](https://book.douban.com/subject/5333562/)
