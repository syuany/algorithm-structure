# tips of cpp

## 赋值运算符重载

### 1. 拷贝赋值运算符 (Copy Assignment Operator)

```cpp
LinkedList &operator=(const LinkedList &other);
```

**功能说明：**

- 用于处理已存在的对象之间的赋值操作
- 当执行 `list1 = list2` 这样的语句时被调用
- 参数是const引用，避免不必要的拷贝同时防止修改源对象

**典型实现示例：**

```cpp
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> &other) {
    if (this != &other) {  // 自赋值检查
        // 清理当前对象的资源
        clear();
        
        // 拷贝other的所有节点
        Node* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }
    return *this;  // 返回当前对象的引用
}
```

### 2. 移动赋值运算符 (Move Assignment Operator)

```cpp
LinkedList &operator=(LinkedList &&other) noexcept;
```

**功能说明：**

- 用于处理临时对象或右值的赋值操作
- 当执行 `list1 = std::move(list2)` 或 `list1 = getList()` 这样的语句时被调用
- `noexcept` 说明该函数不会抛出异常
- 通过"移动语义"避免深拷贝，提高性能

**典型实现示例：**

```cpp
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T> &&other) noexcept {
    if (this != &other) {
        // 清理当前对象的资源
        clear();
        
        // 直接转移资源所有权
        head = other.head;
        size = other.size;
        
        // 将源对象置为空状态
        other.head = nullptr;
        other.size = 0;
    }
    return *this;
}
```

## 使用示例

```cpp
LinkedList<int> list1{1, 2, 3};
LinkedList<int> list2{4, 5, 6};

// 拷贝赋值 - list2的数据被拷贝到list1
list1 = list2;

// 移动赋值 - list2的资源被转移到list1，list2变为空
LinkedList<int> list3 = std::move(list2);
```

这两个运算符是C++"Rule of Five"的重要组成部分，确保类能正确处理对象的赋值操作，特别是资源管理方面。

## why size_t

跨平台

## why const std::string& str and why not const size_t& index

复杂对象传引用减少拷贝，对基本类型无意义
对于基本类型，需要解引用，可能需要内存访问
性能可能反而更差
编译器通常会将其优化为寄存器传递
引用传递可能阻止某些优化

## why sometimes not LinkedList\<T>

在类模板内部中引用自身类型时，可省略，包括初始化列表中

## 拷贝构造函数 (Copy Constructor)

```cpp
LinkedList(const LinkedList &other);
```

特点：

- 深拷贝：创建一个新的独立副本，复制所有数据
- 源对象保持不变：other 对象在操作后仍然有效
- 资源开销大：需要分配新内存并复制所有元素

触发时机：

```cpp
LinkedList<int> list1 = {1, 2, 3};
LinkedList<int> list2(list1);        // 显式调用
LinkedList<int> list3 = list1;       // 隐式调用
func(list1);                         // 参数传递时（按值）
```

移动构造函数 (Move Constructor)

```cpp
LinkedList(LinkedList &&other) noexcept;
```

特点：

- 浅拷贝+转移所有权：直接接管 other 的资源，不进行复制
- 源对象被修改：other 对象在操作后处于有效但未指定状态
- 资源开销小：只需要转移指针，不需要分配新内存
- 触发时机：

```cpp
LinkedList<int> list1 = {1, 2, 3};
LinkedList<int> list2(std::move(list1));  // 显式调用
LinkedList<int> list3 = getList();        // 返回临时对象时
```

## 右值 左值

右值不能被赋值

## why LinkedList &operator=() not LinkedList *operator=()

支持链式赋值语法
保持与内置类型一致的行为
遵循C++语言惯例
提供更好的用户体验

## std::accumulate and std::reduce

reduce(begin, end, none/0LL/...)

后者可并行加速
因此不保证结果有序（用于字符串时）
计算区间 [,)
在 C++ 标准库中，所有范围算法（包括 accumulate、reduce、for_each、find 等）都遵循 左闭右开 原则

## why #ifndef #define

防止头文件被重复包含
对比 #pragma once
\#ifndef 是 C/C++ 标准语法，兼容所有编译器（GCC、Clang、MSVC 等），移植性强。
\#pragma once 是 编译器扩展（非标准，但主流编译器都支持），写法更简洁，但理论上存在极少数兼容性问题（如早期编译器）。

## why using value_type = T

让代码更具 可读性
让代码更容易 维护（修改底层类型时只需改一处）
遵循 STL 容器的接口规范

## iterator::operator++(int)

参数 (int) 是后置递增操作符的标识，不实际使用

## why cbegin() and cend()

明确返回常量迭代器，即使在非const对象上调用
保证遍历时不能修改元素

## why not length = std::move(other.length)

std::move 主要用于将左值转换为右值引用，以便调用移动构造函数或移动赋值运算符。但对于基本类型（如 size_t），移动和拷贝没有区别，因此使用 std::move 是多余的。

## why copy-and-swap

1.异常安全性

- 传统方式：在分配新内存过程中如果出现异常，原对象可能处于不一致状态
- copy-and-swap：具有强异常安全保证，要么成功，要么保持原状态不变
2.代码简洁性

- 传统方式：需要手动处理各种边界情况和内存管理
- copy-and-swap：代码更简洁，复用拷贝构造函数逻辑
3.性能特点

- 传统方式：直接修改当前对象，无需额外对象构造
- copy-and-swap：需要构造临时对象，但通过 swap 操作实现原子性赋值
4.维护性

- 传统方式：需要维护复杂的边界条件处理逻辑
- copy-and-swap：依赖已验证的拷贝构造函数，减少出错可能

## why std::vector::operator[] do not check boundary

- 不强制默认开销，让性能敏感场景受益；
- 兼容 C 数组行为，降低迁移成本；
- 通过 at() 补充安全需求，兼顾不同场景；
- 与标准库其他容器接口一致，降低使用复杂度。

## how to define a multiline string in cpp

string s=R"(第一行内容      // C11以上
第二行内容（可以直接换行）
第三行内容：包含双引号"也不需要转义
第四行内容：甚至可以包含括号()
...
)";

## 左值引用自动转换为右值

int f(){ return int& }  // No Problem

## deal with char / string

isspace(c)
ispunct(c): 符号（非数字非字母）
all_of(first, last, pred): 迭代器范围[first, last)内元素是否全部满足pred函数

## find max element of vector

ranges::max(nums)
*ranges::max_element(nums)
max(nums.begin(), nums.end())
*max_element(nums.begin(), nums.end())

## read data from files

```cpp
#include<fstream>
ifstream file("path");
file >> x;
```

## initialize 2D vector

初始化一个2行2列的二维vector，所有元素为10

```cpp
vector<vector<int>> v2(2, vector<int>(2, 10));
```

## about (? :)

during binary search
nums[m]<t?l:r=m       wrong   => (nums[m]<t) ? l : (r=m)
(nums[m]<t?l:r)=m     correct

## algorithm

### binary search

```cpp
#include<algorithm>
std::binary_search(begin, end, target): 检查元素是否存在 -> bool
std::lower_bound(begin, end, target): (first >=) -> iterator
std::upper_bound(begin, end, target): (first > ) -> iterator
std::equal_range(begin, end, target): [first, second) -> part<iterator, iterator>

std::upper_bound(begin, end, q, [](int val, auto& item){return val < item[0]; })
std::upper_bound(begin, end, q, [](auto& item, int val){return val < item[0]; })
std::upper_bound(begin, end, q, [](int x, int y){return x < y; })

// range: std::vector、std::array 等符合 range 概念的对象
std::ranges::lower_bound(range, tar) -> iterator
```

### prefix sum

```cpp
std::partial_sum(begin, end, tar.begin);
```

### increasing sequence

```cpp
#include<numeric>
std::iota(begin, end, start);   // C++11

int a[5];
std::iota(std::begin(a), std::end(a), start);

// customize step size
#include<algorithm>
int start=0;
std::generate(begin, end, [&start](){ int res=start;start+=2;return res;})
```

## container

### all

所有容器都有：

- TODO

### vector

insert()
    iterator insert(iterator pos, const T& value); // 复制插入（C++98）
    iterator insert(iterator pos, T&& value);      // 移动插入（C++11）
    iterator insert(iterator pos, size_type count, const T& value);
    iterator insert(iterator pos, InputIt first, InputIt last);
    iterator insert(iterator pos, std::initializer_list\<T> ilist);

### unordered_set

insert()

```cpp
// A pair consisting of an iterator to the inserted element 
// (or to the element that prevented the insertion) 
// and a bool value set to true if and only if the insertion took place.
std::pair<iterator,bool> insert( const value_type& value );
std::pair<iterator,bool> insert( value_type&& value );
// for instance
if(!s.insert(nums[i]).second) return true;
```

#### customize hash function to unordered_set

```cpp
struct TupleHash {
    template <class T>
    static void hash_combine(size_t& seed, const T& val) {
        // 参考 boost::hash_combine
        seed ^= hash<T>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    size_t operator()(const tuple<int, int, int>& t) const {
        size_t seed = 0;
        auto& [a, b, c] = t;
        hash_combine(seed, a);
        hash_combine(seed, b);
        hash_combine(seed, c);
        return seed;
    }
};
unordered_set<tuple<int, int, int>, TupleHash> packet_s;
```

#### unordered_map

```cpp
// check without insert
m.contains()        // concisely (C++20)
m.find()!=m.end()
m.count()
```

## assert

assert 是一个用于调试的宏（macro），主要作用是在程序运行时检查某个条件是否为真。
如果条件为假（false），assert 会触发程序终止，并输出详细的错误信息（如文件名、行号、检查的表达式）
assert(a == b && "a cannot equal to b")

## type convert

```cpp
T a = static_cast<T>(x);
```

## math

### MOD

```cpp
MOD = 1_000_000_007

// 加
(a + b) % MOD

// 减，b 在 [0,MOD-1] 中
(a - b + MOD) % MOD

// 把任意整数 a 取模到 [0,MOD-1] 中，无论 a 是正是负
(a % MOD + MOD) % MOD

// 乘（注意使用 64 位整数）
a * b % MOD

// 多个数相乘，要步步取模，防止溢出
a * b % MOD * c % MOD

// 除（MOD 是质数且 b 不是 MOD 的倍数）
a * qpow(b, MOD - 2, MOD) % MOD
```

### ceil

for integer
if y>0
    ceil_div(x, y)=(x+y-1)/y;
if y<0
    ceil_div(x, y)=(x+y+1)/y;

\#include\<cmath>
ceil(double)

## lambda recursive call

auto dfs=[&](this auto&& dfs, ...){};   // C++23

## inline function

- inline 函数是一种用于请求编译器在调用点直接插入函数体代码的优化手段，核心目的是减少函数调用的开销（如栈帧创建、参数传递、返回值处理等），从而提升程序运行效率。
- 如果类的成员函数在类定义内部实现，编译器会默认将其视为 inline 函数（无需显式加 inline）

### 和 #define 的区别

| 特性                | `inline` 函数                          | 宏定义（`#define`）                    |
|---------------------|----------------------------------------|----------------------------------------|
| 类型安全            | 支持参数类型检查，编译报错             | 无类型检查，仅文本替换，易出错         |
| 调试能力            | 可单步调试，保留函数调用栈信息         | 预处理器直接替换，无法调试             |
| 副作用              | 无（参数仅求值一次）                   | 可能有副作用（参数可能多次求值）       |
| 适用场景            | 短小、频繁调用的函数（如访问器、简单计算） | 简单的常量替换、代码片段复用（不推荐用于函数） |

## constexpr

- constexpr 是 C++11 引入的关键字，表示"常量表达式"
- 它告诉编译器某个变量或函数的值可以在编译时确定
- 相比于 const，constexpr 更加强调"编译期常量"

### 与 const 的区别

- const 只保证运行时不变性
- constexpr 保证编译期可求值
- constexpr 隐含了 const 的语义

## attributes

- [[nodiscard]]: 用于标记函数返回值不应该被忽略 (C++17)

## Smart Pointer



## allocator

```cpp
std::allocator_traits<NodeAlloc>::allocate(allocator_, 1);
std::construct_at(ptr, std::forward<Args>(args)...);
std::allocator_traits<NodeAlloc>::destroy(allocator_, ptr);
std::allocator_traits<NodeAlloc>::deallocate(allocator_, ptr, 1);
```

## something about (...)



## Deducing this

### 基本概念

Deducing this 是 C++23 引入的一项新特性，允许在类的非静态成员函数中显式地指定 this 指针的类型。这使得成员函数可以根据调用对象的类型（const、volatile、ref-qualifiers 等）进行重载和选择。

## explicit

explicit 是 C++ 中的一个关键字，用于修饰构造函数，防止该构造函数被隐式调用或用于隐式类型转换

```cpp
Node *node = ...; // 假设有一个 Node 指针
mys::list<int>::iterator it = node; // 隐式调用构造函数
```

这种隐式转换可能会导致代码的可读性和可维护性变差，甚至引入难以发现的错误。通过添加 explicit，可以强制要求调用者显式地构造迭代器

```cpp
Node *node = ...;
mys::list<int>::iterator it(node); // 必须显式调用构造函数
```

## std::conditional_t

### 作用

std::conditional_t 是 C++ 标准库中的一个类型特性（type trait），用于根据条件选择类型。它类似于三元运算符（?:），但用于类型选择。

### 和 std::conditional 的区别

std::conditional 是一个模板结构体，包含一个成员类型 type，而 std::conditional_t 是其简化形式，直接返回所选类型，省略了访问 type 成员的步骤。

```cpp
std::conditional_t<Condition, TypeIfTrue, TypeIfFalse>
```
