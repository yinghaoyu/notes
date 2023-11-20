使用 `pkg-config` 可以查找库依赖
```bash
// 比如使用fmt库
g++ fmtTest.cc -o fmtTest $(pkg-config --cflags --libs fmt)
```
