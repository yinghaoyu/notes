## Profiling简介

explain主要是检查sql语句的基本性能，但不能查看具体的涉及硬件资源的开销，profiling可以更细节的查看资源的开销。

## What

- 查看是否开启了此功能 `show variables like 'profiling%';`默认情况下，参数出于关闭状态，为OFF状态

- 开启Profiling `set profiling=1;`开启此功能后，就能执行我们的SQL语句

- 展示最近的SQL执行情况：show profiles;此命令展示最近执行的sql语句，默认是15次历史记录。

![20210623a](/images/20210623a.png)

`Query_ID`会**一直递增**，这个id可以查询某一条SQL的执行耗时清单

![20210623b](/images/20210623b.png)

- 具体查看某一条SQL的执行细节：show profile cpu,block io for query 2;

![20210623c](/images/20210623c.png)

可以使用的类型如下：

> `ALL` 显示所有的开销信息
> 
> `BLOCK IO` 显示块IO相关开销
> 
> `CONTEXT SWITCHES` 上下文切换相关开销
> 
> `CPU` 显示CPU相关开销
> 
> `IPC` 显示发送和接受相关开销
> 
> `MEMORY` 显示内存相关开销
> 
> `PAGE FAULTS` 显示页面错误相关开销
> 
> `SOURCE` 显示和Source_function, Source_file,Source_line相关的相关开销
> 
> `SWAPS` 显示交换次数相关开销

## 参考文献

1. Ben Forta. [MySQL必知必会. 人民邮电出版社, 2009](https://book.douban.com/subject/3354490/)
2. 简朝阳. [MySQL性能调优与架构设计. 电子工业出版社, 2009](https://book.douban.com/subject/3729677/)
