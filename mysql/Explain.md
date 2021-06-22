[Explain examples](https://www.php.cn/mysql-tutorials-454417.html)

## explain作用
通过explain可以分析查询语句或表结构的性能瓶颈。可以得出以下结果：
- 表的读取顺序
- 数据读取操作的操作类型
- 哪些索引可以使用
- 哪些索引被实际使用
- 表之间的引用
- 每张表有多少行被优化器查询
## 执行计划各字段含义
1. id

> select查询的序列号，包含一组数字，表示查询中执行select子句或操作表的顺序

id的结果共有3种情况

- id相同，执行顺序由上至下

![20210622a](/images/20210622a.png)

- id不同，如果是子查询，id的序号会递增，**id值越大优先级越高，越先被执行**

![20210622b](/images/20210622b.png)

- id相同不同，同时存在

![20210622c](/images/20210622c.png)

id如果相同可以认为是同一组，从上往下顺序执行，在所有组中id值越大，优先级越高，越先执行，derived = 衍生

如上图所示，在id为1时，table显示的是<derived2>，这里指的是指向id为2的表，即t3表的衍生表。

2. select_type

常见和常用的值有如下几种：

- SIMPLE `简单的select查询`，查询中`不包含子查询或者UNION`

- PRIMARY 查询中若`包含任何复杂的`子部分，`最外层查询则被标记为PRIMARY`

- SUBQUERY `在SELECT或WHERE列表中包含了子查询`

- DERIVED 在FROM列表中包含的`子查询被标记为DERIVED`（衍生），MySQL会递归执行这些子查询，把`结果放在临时表`中

- UNION 若第二个SELECT出现在UNION之后，则被标记为UNION：若UNION包含在FROM子句的子查询中，外层SELECT将被标记为：DERIVED

- UNION RESULT 从UNION表获取结果的SELECT

分别用来表示查询的类型，主要是用于区别普通查询、联合查询、子查询等的复杂查询。

3. table

指的就是当前执行的表
  
4. type
  
从最好到最差依次是：

system > const > eq_ref > ref > range > index > all
