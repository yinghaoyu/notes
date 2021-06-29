#ifndef GUARD_BIND_T_H
#define GUARD_BIND_T_H

template<int I>
class placeholder{};

placeholder<1> _1;
placeholder<2> _2;
placeholder<3> _3;

template<typename A1, typename A2>
class List2
{
public:
  List2(A1 a1, A2 a2):a1_(a1), a2_(a2)
  {
  }

  ~List2(){}

  // 在重载解析的时候，普通函数的优先级高于模板函数
  A1 operator[](placeholder<1>) const { return a1_; } // 如果是占位符

  A2 operator[](placeholder<2>) const { return a2_; } // 如果是占位符

  template <typename T>
  T operator[](T v) const { return v; }   // 如果是普通的类型

  template<typename F, typename L>
  void operator()(F f, L l)
  {
    f(l[a1_], l[a2_]);  // 这里的根据 a1，a2 的类型得到实际的值
  }

private:
  A1 a1_;
  A2 a2_;
};

template<typename A1>
class List1
{
public:
  List1(A1 a1):a1_(a1)
  {
  }

  ~List1(){}

  // 在重载解析的时候，普通函数的优先级高于模板函数
  A1 operator[](placeholder<1>) const { return a1_; } // 如果是占位符

  template <typename T>
  T operator[](T v) const { return v; }   // 如果是普通的类型

  template<typename F, typename L>
  void operator()(F f, L l)
  {
    f(l[a1_]);  // 这里的根据 a1 的类型得到实际的值
  }

private:
  A1 a1_;
};

template<typename F, typename L>
class bind_t
{
public:
  bind_t(F f, L l):f_(f), l_(l)
  {
  }

  ~bind_t(){}

  template<typename A>
  void operator()(A a)
  {
    List1<A> list(a);
    l_(f_, list);
  }

  template<typename A1, typename A2>
  void operator()(A1 a1, A2 a2)
  {
    List2<A1, A2> list(a1, a2);
    l_(f_, list);
  }
private:
  F f_;
  L l_;
};

#endif
