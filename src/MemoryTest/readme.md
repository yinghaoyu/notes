### MemoryTest

虚拟地址空间是每个进程的，而物理地址空间则是所有进程共享的。换句话说，物理地址是全局的。

#### 测试步骤
先运行`test` 让`test`停留在`getchar()`函数， 再根据`test`的输出作为`getphys`的输入,

再根据`getphys`的输出作为内核模块`test.ko`的输入,

最后输入任意字符，发现test的输出字符串改变了。

```
./test

./getphys 10268 140433055821824

insomd kernelTest.ko addr=5754781696
```

输出
```
addr: 140433055821824   pid:10268
before:Zhejiang wenzhou pixie shi

after:rain flooding water will not get fat!
```
