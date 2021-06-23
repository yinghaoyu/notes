## explain作用
通过explain可以分析查询语句或表结构的性能瓶颈。可以得出以下结果：
- 表的读取顺序
- 数据读取操作的操作类型
- 哪些索引可以使用
- 哪些索引被实际使用
- 表之间的引用
- 每张表有多少行被优化器查询
## 执行计划各字段含义
### 1.id

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

### 2.select_type
  
常见和常用的值有如下几种：

  - SIMPLE `简单的select查询`，查询中`不包含子查询或者UNION`

  - PRIMARY 查询中若`包含任何复杂的`子部分，`最外层查询则被标记为PRIMARY`

  - SUBQUERY `在SELECT或WHERE列表中包含了子查询`

  - DERIVED 在FROM列表中包含的`子查询被标记为DERIVED`（衍生），MySQL会递归执行这些子查询，把`结果放在临时表`中

  - UNION 若第二个SELECT出现在UNION之后，则被标记为UNION：若UNION包含在FROM子句的子查询中，外层SELECT将被标记为：DERIVED
  
  - UNION RESULT 从UNION表获取结果的SELECT

  分别用来表示查询的类型，主要是用于区别普通查询、联合查询、子查询等的复杂查询。

### 3.table

指的就是当前执行的表

### 4.type

从最好到最差依次是：

> system > const > eq_ref > ref > range > index > all
  
**一般来说，得保证查询至少达到range级别，最好能达到ref。**

- `system` 表只有一行记录（等于系统表），这是const类型的特列，平时不会出现，这个也可以忽略不计
  
- `const` 表示通过索引一次就找到了，const用于比较primary key 或者unique索引。因为只匹配一行数据，所以很快。如将主键置于where列表中，MySQL就能将该查询转换为一个常量。
  
![20210622d](/images/20210622d.png)
  
首先进行子查询得到一个结果的d1临时表，子查询条件为id = 1 是常量，所以type是const，id为1的相当于只查询一条记录，所以type为system。

- `eq_ref` 唯一性索引扫描，对于每个索引键，表中只有一条记录与之匹配。常见于主键或唯一索引扫描

- `ref` 非唯一性索引扫描，返回匹配某个单独值的所有行，本质上也是一种索引访问，它返回所有匹配某个单独值的行，然而，它可能会找到多个符合条件的行，所以他应该属于查找和扫描的混合体。
  
![20210622e](/images/20210622e.png)
  
- `range` 只检索给定范围的行，使用一个索引来选择行，key列显示使用了哪个索引，一般就是在你的where语句中出现between、< 、>、in等的查询，这种范围扫描索引比全表扫描要好，因为它只需要开始于索引的某一点，而结束于另一点，不用扫描全部索引。
  
![20210622f](/images/20210622f.png)

- `index` Full Index Scan，Index与All区别为index类型只遍历索引树。这通常比ALL快，因为索引文件通常比数据文件小。（也就是说虽然all和Index都是读全表，但index是从索引中读取的，而all是从硬盘读取的）

![20210622g](/images/20210622g.png)
  
id是主键，所以存在主键索引
- `all` Full Table Scan 将遍历全表以找到匹配的行
  
![20210622h](/images/20210622h.png)

### 5.possible_keys 和 key

`possible_keys` 显示可能应用在这张表中的索引，一个或多个。查询涉及到的字段上若存在索引，则该索引将被列出，**但不一定被查询实际使用**。

`key`

- 实际使用的索引，如果为NULL，则没有使用索引。（可能原因包括没有建立索引或索引失效）

![20210622i](/images/20210622i.png)

- 查询中若使用了`覆盖索引`（select 后要查询的字段刚好和创建的索引字段完全相同），则该索引仅出现在key列表中

![20210622j](/images/20210622j.png)

### 6.key_len

表示索引中使用的字节数，可通过该列计算查询中使用的索引的长度，**在不损失精确性的情况下，长度越短越好**。key_len显示的值为索引字段的最大可能长度，并非实际使用长度，即key_len是根据表定义计算而得，不是通过表内检索出的。

![20210622k](/images/20210622k.png)

### 7.ref

显示索引的那一列被使用了，如果可能的话，最好是一个常数。哪些列或常量被用于查找索引列上的值。


### 8.rows

根据表统计信息及索引选用情况，大致估算出找到所需的记录所需要读取的行数，也就是说，用的越少越好

![20210622m](/images/20210622m.png)

### 9.Extra

包含不适合在其他列中显式但十分重要的额外信息

- Using filesort

说明mysql会对数据使用一个外部的索引排序，而不是按照表内的索引顺序进行读取。MySQL中无法利用索引完成的排序操作称为“文件排序”。

![20210622n](/images/20210622n.png)

索引字段为col1和col2，最后一条语句没有按索引顺序使用，只使用了col3，因此是相对无序的，需要排序（filesort）。如果恰好是**有序的就不需要排序**（没有filesort），比如前面两条语句。

- Using temporary

使用了用临时表保存中间结果，MySQL在对查询结果排序时使用临时表。常见于排序order by和分组查询group by。

![20210622o](/images/20210622o.png)

- Using index

表示相应的select操作中使用了覆盖索引（Covering Index），避免访问了表的数据行，效率不错。如果同时出现using where，表明索引被用来执行索引键值的查找；如果没有同时出现using where，表明索引用来读取数据而非执行查找动作。
  
![20210622p](/images/20210622p.png)![20210622q](/images/20210622q.png)
  
- Using where

表明使用了where过滤

- Using join buffer

表明使用了连接缓存,比如说在查询的时候，多表join的次数非常多，那么将配置文件中的缓冲区的join buffer调大一些。

- impossible where

where子句的值**总是false**，不能用来获取任何元组

> select * from t1 where id = '1' and id = '2'

- select tables optimized away
  
在没有GROUPBY子句的情况下，基于索引优化MIN/MAX操作或者对于MyISAM存储引擎优化COUNT(*)操作，不必等到执行阶段再进行计算，查询执行计划生成的阶段即完成优化。

- distinct
  
优化distinct操作，在找到第一匹配的元组后即停止找同样值的动作
  
## 参考文献

1. Ben Forta. [MySQL必知必会. 人民邮电出版社, 2009](https://book.douban.com/subject/3354490/)
2. 简朝阳. [MySQL性能调优与架构设计. 电子工业出版社, 2009](https://book.douban.com/subject/3729677/)
3. 青灯夜游. [MySQL中explain用法和结果分析](https://www.php.cn/mysql-tutorials-454417.html)
