## 将**变化相对较少的部分活跃数据**通过应用层的Cache机制Cache到内存中

  1. 系统各种配置及规则数据；由于这些配置信息变动的频率非常低，访问概率又很高，所以非常适合存使用 Cache;
  
  2. 活跃用户的基本信息数据；修改次数少，且频繁访问。
  
  3. 活跃用户的个性化定制信息数据；没有用户的基本信息那么的频繁，但相对
  于系统整体来说，也占了很大的比例，而且变更皮律一样不会太多。从 Ebay 的 PayPal 通过
  MySQL 的 Memory 存储引擎实现用户个性化定制数据的成功案例我们就能看出对这部分信息进行
  Cache 的价值。
  
  4. 准实时的统计信息数据；基于时间段的统计数据。这种数据不会实时更新，也很少
需要增量更新，只有当达到重新 Build 该统计数据的时候需要做一次全量更新操作。

  5. 其他一些访问频繁但变更较少的数据；

## Case 1

实现每个用户查看各自相册列表（假设每个列表显示 10 张相片）的时候，能够在相片名称后面显示该相片的留言数量。

方案一：
```sql
SELECT id,subject,url FROM photo WHERE user_id = ? limit 10;

SELECT COUNT(*) FROM photo_comment WHERE photh_id = ? //此条执行10次，过度依赖面向对象思想，过渡弱化 SQL 语句的功能造成的资源浪费
```

方案二：
```sql
SELECT id,subject,url FROM photo WHERE user_id = ? limit 10;

SELECT photo_id,count(*) FROM photo_comment WHERE photo_id in (?) GROUP BY photo_id  //此条执行1次 
```
> MySQL 对客户端每次提交的 SQL 不管是相同还是不同，都需要进行**完全解析**，解析动作在整个 SQL 语句执行过程中的整体消耗的 CPU 比例是较多的。

## Case 2
在群组简介页面需要显示群名称和简介，每个群成员的nick_name，以及群主的个人签名信息。

方案一：
```sql
SELECT name,description,user_type,nick_name,sign
FROM groups,user_group,user ,user_profile
WHERE groups.id = ?
AND groups.id = user_group.group_id
AND user_group.user_id = user.id
AND user_profile.user_id = user.id
```

方案二：
```sql
SELECT name,description,user_type,nick_name
FROM groups,user_group,user
WHERE groups.id = ?
AND groups.id = user_group.group_id
AND user_group.user_id = user.id

SELECT sign FROM user_profile WHERE user_id = ?
```
> 方案一对user_profile表有不必要的访问（非群主的profile信息），造成IO访问的直接增加。

## 充分利用 Explain 和 和  Profiling

```sql
explain select * from obj_table where id = 1;

set profiling = 1;  // 开启剖析功能
select select * from obj_table where id = 1;
show profiles;
show profile cpu,block io for query 1;  // 针对单个 Query 获取详细的 profile 信息，1为show profiles中得到的某个id

show processlist; // 查看数据库连接
```
