## 如何理解“重载决议是在权限判定之前”？
```cpp
class A
{
public:
  void fun(void*){}
private:
  void fun(char*){}
};
int main()
{
  char str[] = "hello world";
  A a;
  a.fun(str);  // 编译出错，提示不能访问私有成员
  return 0;
}
```
