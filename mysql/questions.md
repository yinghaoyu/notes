#### 1.什么是回表？

答：mysql使用非聚簇索引（二级）索引key查询二级索引树，查询得到聚簇（一级）索引key，然后去聚簇索引树查找得到数据。也就是说，基于非聚簇索引的查询需要多扫描一棵索引树。

#### 2.什么是覆盖索引？

答：只需要在一棵索引树上就能获取SQL所需的所有列数据，无需回表，速度更快。
```
create table mytb(
    id int primary key, 
    k int not null, 
    name varchar(16),
    age int,
    index (k)
)engine = InnoDB;
```
`select id from mytb where k = 2;`二级索引树k存的值就是id，因此不需要搜索id的索引树，这就是索引覆盖

`select * form mytb where k = 2;`此时就变成了回表

#### 3.什么是最左匹配？
答：最左匹配原则就是指在联合索引中，如果你的 SQL 语句中用到了联合索引中的最左边的索引，那么这条 SQL 语句就可以利用这个联合索引去进行匹配。

`select name, age from mytb where name = '';`如果创建了index(name,age)的索引，则能使用name;如果创建了index(age,name)就不能用

`select name, age from mytb where age = '';`如果创建了index(name,age)的索引就不能用;如果创建了index(age,name)，则能用age

`select name, age from mytb where age = '' and name = '';`如果创建了index(age,name)索引，能用

`select name, age from mytb where age = '' and name = '';`如果创建了index(name,age)索引，也能用，因为优化器会将条件顺序优化为先查name后查age

#### 4.以下查询条件创建索引，方案1和方案2哪个好？
```
select name, age from mytb where name = '' and age = '';
select name, age from mytb where name = '';
select name, age from mytb where age = '';
```
方案1: index(name, age) index(age)

方案2：index(age, name) index(name)

答：方案1更好，因为正常情况下name的长度比age长，建立name的索引浪费的空间比age大，网络io也更大。

#### 5.什么是索引下推？

答：索引下推默认开启，搜索的不符合条件的数据在数据库引擎就过滤掉，而不是在server层过滤，减少了网络io。
