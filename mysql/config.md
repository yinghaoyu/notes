vim /etc/mysql/mysql.conf.d/mysqld.cnf 
用#注释bind-address = 127.0.0.1 ,可以使用对外ip连接mysql

任意主机以用户root和密码123456连接到mysql服务器
```
mysql> GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY '123456' WITH GRANT OPTION;
mysql> flush privileges;
```
