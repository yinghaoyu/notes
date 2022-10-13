vim /etc/mysql/mysql.conf.d/mysqld.cnf 
用#注释bind-address = 127.0.0.1 ,可以使用对外ip连接mysql

任意主机以用户root和密码123456连接到mysql服务器
```
 mysql> use mysql;
 mysql> ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '123456';
 mysql> flush privileges;
 mysql> quit;
 
 sudo service mysql restart
```
