```sql
-- IP转数字
inet_aton('255.255.255.255');
INSERT INTO ipt VALUES (inet_aton('192.168.101.88'))
-- 数字转IP
inet_ntoa(4294967295)
SELECT inet_ntoa(ip) FROM ipt;


-- 使用int类型存储IP比varchar类型存储IP地址性能要提升很多，减少不少空间。因为varchar是可变长形，需要多余的一个字节存储长度。另外int型在逻辑运算上要比varchar速度快。
```
