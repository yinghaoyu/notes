```sql
begin;
select * from t where c=5 for update;
commit;
历史知识的结论是，innodb先锁全表的所有行，返回server层，判断c是否等于5，然后释放c！=5的行锁。
验证方法：
事务A执行 锁住一行c！=5的记录 比如id =3 c=3
select * from t where id = 3 for update 或者 update t set c=4 where id =3
然后启动新事务B执行上面的语句select * from t where c=5 for update; 看看有没有被阻塞。
用于判断事务B的语句会不会试图锁不满足条件的记录。
然后把事务A和事务B的执行顺序对调一下，也就是先执行B在执行A。看看有没有阻塞，
判断在事务B加锁成功的情况下会不会释放不满足查询条件记录的行锁。
```

```sql
//https://funnylog.gitee.io/mysql45/19%E8%AE%B2%E4%B8%BA%E4%BB%80%E4%B9%88%E6%88%91%E5%8F%AA%E6%9F%A5%E4%B8%80%E8%A1%8C%E7%9A%84%E8%AF%AD%E5%8F%A5%EF%BC%8C%E4%B9%9F%E6%89%A7%E8%A1%8C%E8%BF%99%E4%B9%88%E6%85%A2.html
select * from t where id=1; 是一致性读，可能会触发 undo，导致查询变慢
select * from t where id=1 lock in share mode; 是当前读
```

```
锁就是加在索引上的，这是InnoDB的一个基础设定
```
