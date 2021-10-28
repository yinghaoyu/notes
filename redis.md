## redis6.2.6集群搭建
至少6个节点（3主3从）
### 启动节点
redis-server 7100.conf
redis-server 7200.conf
redis-server 7300.conf
### 节点握手
redis-cli -p 7001 -h 127.0.0.1 -a 123456 -c# -c 选项指定以Cluster模式运行
127.0.0.1:7100> CLUSTER MEET 127.0.0.1 7100 OK 
127.0.0.1:7100> CLUSTER MEET 127.0.0.1 7200 OK
127.0.0.1:7100> CLUSTER MEET 127.0.0.1 7300 OK
### 查看节点
127.0.0.1:7100> CLUSTER NODES
### 查看信息（包含节点id）
127.0.0.1:7100> CLUSTER INFO
### 主节点指派16384个槽
redis-cli  -p 7100 -h 127.0.0.1 -a 123456 cluster addslots {0..5000}
redis-cli  -p 7200 -h 127.0.0.1 -a 123456 cluster addslots {5001..10000}
redis-cli  -p 7300 -h 127.0.0.1 -a 123456 cluster addslots {10001..16383}
### 主从复制
redis-cli  -p 7400 -h 127.0.0.1 -a 123456 cluster replicate 87b7dfacde34b3cf57d5f46ab44fd6fffb2e4f52(主节点7001)
redis-cli  -p 7500 -h 127.0.0.1 -a 123456 cluster replicate c47598b25205cc88abe2e5094d5bfd9ea202335f(主节点7002)
redis-cli  -p 7600 -h 127.0.0.1 -a 123456 cluster replicate 51081a64ddb3ccf5432c435a8cf20d45ab795dd8(主节点7003)
实现主从复制后，高可用完成，此时假如7200宕机，7500会升主，7200恢复后会成为7500的slave

127.0.0.1:7200>add name yuyinghao
查看键槽
127.0.0.1:7200>cluster keyslot name


### 节点扩张
启动节点+节点握手
### 添加集群
redis-cli -a 123456 --cluster add-node 127.0.0.1:8100 127.0.0.1:7100
redis-cli -a 123456 --cluster add-node 127.0.0.1:8200 127.0.0.1:7100
### 重新分片
redis-cli -a 123456 --cluster reshard 127.0.0.1 7100

How many slots move?4096(16384/4)4个主节点均分
What is the receiving node ID?4297a5a4a632c3978c7001ffa0047b288df43333（8100接收）
### 移出节点
87b7dfacde34b3cf57d5f46ab44fd6fffb2e4f52（7100）
c47598b25205cc88abe2e5094d5bfd9ea202335f(7200)
51081a64ddb3ccf5432c435a8cf20d45ab795dd8(7300)
也可以选all全部重新分配
设置主从
redis-cli  -p 8200 -h 127.0.0.1 -a 123456 cluster replicate 4297a5a4a632c3978c7001ffa0047b288df43333（8100）

### 节点收缩
将7100、7200节点删除
重新分片
redis-cli -a 123456 --cluster reshard 127.0.0.1 7100

How many slots move?4096
What is the receiving node ID?4297a5a4a632c3978c7001ffa0047b288df43333（8100接收）
移出节点
87b7dfacde34b3cf57d5f46ab44fd6fffb2e4f52（7100）
done
移出后，可以发现原7100的从节点7400，变成了8100的主节点

依次删除
redis-cli -a 123456 --cluster del-node 127.0.0.1:7400 20a7e14816ff967407e691
bd543250004bb9a1e3（7400）

redis-cli -a 123456 --cluster del-node 127.0.0.1:7400 20a7e14816ff967407e691
bd543250004bb9a1e3（7100）
