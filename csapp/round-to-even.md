- **向零舍入**

	- 把正数向下舍入，把负数向上舍入，得到值x^，使得|x^| <= |x|。

- **向下舍入**

	- 把正数和负数都向下舍入，得到值x-，使得x- <= x。

- **向上舍入**

	- 把正数和负数都向上舍入，得到值x+，使得x <= x+。

- **向偶数舍入**

	向偶数舍入有两个原则
	
	- **向最接近的值**舍入。
	
	- 当处在"中间值"时**看有效数值是否是偶数**，如果是偶数则直接舍去不进位，如果是奇数则进位。

先拿几个十进制的举例，再看二进制的。
	
比如以下几个十进制都要求舍入到个位：1.4和1.6，所谓的"中间值"就是，比如1.4，因为是舍入到个位，所以它处在1和2之间，那中间值就是(1+2)/2=1.5。
	
那么1.4就不是中间值，根据向最接近的值舍入的原则1.4跟1更近，所以舍入得到1。
	
同理，1.6舍入得到2。再看1.5和2.5：

根据上述可知，1.5和2.5都是中间值，那么就要看有效数字是否是偶数，1.5的有效数字是1，是奇数，所以要进位得到2；而2.5的有效数字是2，所以直接舍去不进位得到2。

- - - 

二进制也是相同的道理，二进制的基数是2，半值就这么算(2<sup>n</sup>)/2，然后二进制的奇偶判断看最有效位，0则偶，1则奇。
	
二进制10.010，舍入到小数点下一位，那么看有效数字后面的数字是10，半值(2<sup>2</sup>)/2=2=二进制的10，正好是半值，那么看有效数字是10.0，是偶数，所以直接舍去得10.0。

再看10.011，半值同样是10，而有效数字后面的11比10大则直接进位得10.1。
	
再看10.110，半值同样是10，有效数字后面的10正好是半值，看有效数字10.1是奇数，那么要进1位，得11.0。

> 因此，浮点数运算是不支持结合律的。例如(3.14 + 1e10) - 1e10结果和3.14 +（1e10 - 1e10）是不一样的，存在舍入或者溢出。
