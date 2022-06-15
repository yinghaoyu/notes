# GCC编译选项

- 检查地址访问越界`-fsanitize=address`
- 检查线程数据竞态`-fsanitize=thread`
- 使编译后的汇编指令严格排序，编译优化可能会调整指令顺序`-fno-toplevel-reorder`
- 开启gdb调试选项`-ggdb`
- 显示所有警告`-Wall`
- 显示-Wall的额外警告`-Wextra`
- 把警告当错误处理`-Werror`
- 警告可能改变值的隐式转换`-Wconversion`
- 如果使用旧样式（C 样式）强制转换为非void类型，则发出警告`-Wold-style-cast`
- 如果基类函数被子类函数隐藏，就引发编译错误，注意覆盖、隐藏、重载区别`-Woverloaded-virtual`
- 指定语言标准为c++11`-std=c++11`
