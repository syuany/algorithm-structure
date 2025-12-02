#include "forward_list.h"
#include <iostream>
#include <cassert>
#include <string>
#include <algorithm>

// 测试辅助函数
template <typename T>
void print_forward_list(const mys::forward_list<T> &list, const std::string &name) {
    std::cout << name << ": ";
    for (const auto &item : list) {
        std::cout << item << " ";
    }
    std::cout << "(size: " << list.size() << ")" << std::endl;
}

// 测试用的自定义类型
struct TestStruct {
    int value;

    TestStruct(int v = 0) : value(v) {}

    bool operator==(const TestStruct &other) const { return value == other.value; }

    auto operator<=>(const TestStruct &other) const = default;
};

std::ostream &operator<<(std::ostream &os, const TestStruct &ts) {
    return os << ts.value;
}

void test_constructors_and_assignment() {
    std::cout << "\n=== Testing Constructors and Assignment ===" << std::endl;

    // 默认构造函数
    mys::forward_list<int> list1;
    assert(list1.empty());
    assert(list1.size() == 0);
    std::cout << "Default constructor: OK" << std::endl;

    // 初始化列表构造函数
    mys::forward_list<int> list2 = {1, 2, 3, 4, 5};
    assert(!list2.empty());
    assert(list2.size() == 5);
    assert(list2.front() == 1);
    std::cout << "Initializer list constructor: OK" << std::endl;

    // 拷贝构造函数
    mys::forward_list<int> list3(list2);
    assert(list3.size() == 5);
    assert(list3.front() == 1);
    std::cout << "Copy constructor: OK" << std::endl;

    // 移动构造函数
    mys::forward_list<int> list4(std::move(list2));
    assert(list4.size() == 5);
    assert(list4.front() == 1);
    // list2现在处于有效但未指定状态，但我们期望它是空的
    std::cout << "Move constructor: OK" << std::endl;

    // 拷贝赋值运算符
    mys::forward_list<int> list5;
    list5 = list3;
    assert(list5.size() == 5);
    assert(list5.front() == 1);
    std::cout << "Copy assignment: OK" << std::endl;

    // 移动赋值运算符
    mys::forward_list<int> list6;
    list6 = std::move(list3);
    assert(list6.size() == 5);
    assert(list6.front() == 1);
    std::cout << "Move assignment: OK" << std::endl;
}

void test_element_access() {
    std::cout << "\n=== Testing Element Access ===" << std::endl;

    mys::forward_list<int> list = {10, 20, 30};

    // 测试 front()
    assert(list.front() == 10);
    const auto &clist = list;
    assert(clist.front() == 10);
    std::cout << "front(): OK" << std::endl;

    // 测试空列表访问（应该抛出异常）
    //     mys::forward_list<int> empty_list;
    //     try {
    //         empty_list.front();
    //         assert(false); // 应该不会到达这里
    //     } catch (const std::out_of_range &) {
    //         std::cout << "front() on empty list throws exception: OK" << std::endl;
    //     }
}

void test_capacity_queries() {
    std::cout << "\n=== Testing Capacity Queries ===" << std::endl;

    mys::forward_list<int> list;

    // 空列表
    assert(list.empty());
    assert(list.size() == 0);
    std::cout << "Empty list capacity: OK" << std::endl;

    // 非空列表
    list.push_front(1);
    assert(!list.empty());
    assert(list.size() == 1);
    std::cout << "Non-empty list capacity: OK" << std::endl;

    // 多元素列表
    list.push_front(2);
    list.push_front(3);
    assert(!list.empty());
    assert(list.size() == 3);
    std::cout << "Multi-element list capacity: OK" << std::endl;
}

void test_modifiers() {
    std::cout << "\n=== Testing Modifiers ===" << std::endl;

    mys::forward_list<int> list;

    // push_front
    list.push_front(1);
    assert(list.front() == 1);
    assert(list.size() == 1);

    list.push_front(2);
    assert(list.front() == 2);
    assert(list.size() == 2);
    std::cout << "push_front: OK" << std::endl;

    // emplace_front
    list.emplace_front(3);
    assert(list.front() == 3);
    assert(list.size() == 3);
    std::cout << "emplace_front: OK" << std::endl;

    // pop_front
    list.pop_front();
    assert(list.front() == 2);
    assert(list.size() == 2);

    list.pop_front();
    assert(list.front() == 1);
    assert(list.size() == 1);

    list.pop_front();
    assert(list.empty());
    assert(list.size() == 0);
    std::cout << "pop_front: OK" << std::endl;

    // pop_front on empty list (should throw)
    // try {
    //     list.pop_front();
    //     assert(false);
    // } catch (const std::out_of_range &) {
    //     std::cout << "pop_front on empty list throws exception: OK" << std::endl;
    // }

    // clear
    mys::forward_list<int> list2 = {1, 2, 3, 4, 5};
    list2.clear();
    assert(list2.empty());
    assert(list2.size() == 0);
    std::cout << "clear: OK" << std::endl;
}

void test_insert_and_erase() {
    std::cout << "\n=== Testing Insert and Erase ===" << std::endl;

    mys::forward_list<int> list;

    // 在空列表中插入第一个元素
    auto it = list.insert_after(list.before_begin(), 1);
    assert(list.front() == 1);
    assert(*it == 1);
    assert(list.size() == 1);
    std::cout << "insert_after at beginning: OK" << std::endl;

    // 在第一个元素之后插入元素
    it = list.insert_after(list.begin(), 2);
    assert(list.front() == 1);
    assert(*it == 2);
    assert(list.size() == 2);
    // 此时列表是: 1 -> 2
    std::cout << "insert_after in middle: OK" << std::endl;

    // 在开头插入元素
    it = list.emplace_after(list.before_begin(), 3);
    assert(*it == 3);
    assert(list.size() == 3);
    assert(list.front() == 3);
    // 此时列表是: 3 -> 1 -> 2
    std::cout << "emplace_after: OK" << std::endl;

    // 删除第一个元素(3)
    it = list.erase_after(list.before_begin());
    assert(list.front() == 1);
    assert(*it == 1);
    assert(list.size() == 2);
    // 此时列表是: 1 -> 2
    std::cout << "erase_after single element: OK" << std::endl;

    // 再在开头插入两个元素
    list.push_front(4);
    list.push_front(5);
    // 此时列表是: 5 -> 4 -> 1 -> 2
    assert(list.size() == 4);
    assert(list.front() == 5);

    // 准备范围删除: 删除4和1这两个元素
    auto first = list.begin();       // 指向 5
    auto last = std::next(first, 3); // 指向 2

    // 删除 (first, last) 即 4 , 1
    it = list.erase_after(first, last);
    assert(*it == 2); // 返回指向被删除范围后第一个元素的迭代器
    assert(list.size() == 2);
    assert(list.front() == 5); // 剩下的列表是: 5 -> 2
    std::cout << "erase_after range: OK" << std::endl;
}

void test_iterators() {
    std::cout << "\n=== Testing Iterators ===" << std::endl;

    mys::forward_list<int> list = {1, 2, 3, 4, 5};

    // 正向迭代器
    int expected = 1;
    for (auto it = list.begin(); it != list.end(); ++it) {
        assert(*it == expected++);
    }
    std::cout << "Forward iteration: OK" << std::endl;

    // const迭代器
    const mys::forward_list<int> clist = {1, 2, 3};
    expected = 1;
    for (auto it = clist.cbegin(); it != clist.cend(); ++it) {
        assert(*it == expected++);
    }
    std::cout << "Const iteration: OK" << std::endl;

    // before_begin
    auto bit = list.before_begin();
    assert(++bit == list.begin());
    std::cout << "before_begin: OK" << std::endl;
}

void test_operations() {
    std::cout << "\n=== Testing Operations ===" << std::endl;

    // splice_after
    mys::forward_list<int> list1 = {1, 2, 3};
    mys::forward_list<int> list2 = {4, 5, 6};

    list1.splice_after(list1.before_begin(), list2);
    assert(list1.size() == 6);
    assert(list2.size() == 0);
    assert(list1.front() == 4);
    std::cout << "splice_after entire list: OK" << std::endl;

    // remove
    mys::forward_list<int> list3 = {1, 2, 3, 2, 4, 2, 5};
    list3.remove(2);
    assert(list3.size() == 4);
    // 验证2已被移除
    for (const auto &item : list3) {
        assert(item != 2);
    }
    std::cout << "remove: OK" << std::endl;

    // remove_if
    mys::forward_list<int> list4 = {1, 2, 3, 4, 5, 6};
    list4.remove_if([](int n) { return n % 2 == 0; }); // 移除偶数
    assert(list4.size() == 3);
    for (const auto &item : list4) {
        assert(item % 2 != 0);
    }
    std::cout << "remove_if: OK" << std::endl;

    // unique
    mys::forward_list<int> list5 = {1, 1, 2, 2, 3, 3, 3, 4, 5, 5};
    list5.unique();
    assert(list5.size() == 5);
    int expected[] = {1, 2, 3, 4, 5};
    int i = 0;
    for (const auto &item : list5) {
        assert(item == expected[i++]);
    }
    std::cout << "unique: OK" << std::endl;

    // reverse
    mys::forward_list<int> list6 = {1, 2, 3, 4, 5};
    list6.reverse();
    int rev_expected[] = {5, 4, 3, 2, 1};
    i = 0;
    for (const auto &item : list6) {
        assert(item == rev_expected[i++]);
    }
    std::cout << "reverse: OK" << std::endl;
}

void test_comparison_operators() {
    std::cout << "\n=== Testing Comparison Operators ===" << std::endl;

    mys::forward_list<int> list1 = {1, 2, 3};
    mys::forward_list<int> list2 = {1, 2, 3};
    mys::forward_list<int> list3 = {1, 2, 4};
    mys::forward_list<int> list4 = {1, 2};

    // ==
    assert(list1 == list2);
    assert(!(list1 == list3));
    std::cout << "operator==: OK" << std::endl;

    // <=>
    assert((list1 <=> list2) == std::strong_ordering::equal);
    assert((list1 <=> list3) == std::strong_ordering::less);
    assert((list3 <=> list1) == std::strong_ordering::greater);
    assert((list1 <=> list4) == std::strong_ordering::greater);
    std::cout << "operator<=>: OK" << std::endl;
}

void test_custom_types() {
    std::cout << "\n=== Testing Custom Types ===" << std::endl;

    mys::forward_list<TestStruct> list;
    list.push_front(TestStruct(10));
    list.push_front(TestStruct(20));

    assert(list.front().value == 20);
    assert(list.size() == 2);
    std::cout << "Custom types: OK" << std::endl;
}

void test_edge_cases() {
    std::cout << "\n=== Testing Edge Cases ===" << std::endl;

    // 空列表的各种操作
    mys::forward_list<int> empty_list;
    assert(empty_list.empty());
    assert(empty_list.size() == 0);

    // begin == end 对于空列表
    assert(empty_list.begin() == empty_list.end());
    std::cout << "Empty list iterators: OK" << std::endl;

    // 单元素列表
    mys::forward_list<int> single_list;
    single_list.push_front(42);
    assert(single_list.size() == 1);
    assert(single_list.front() == 42);
    assert(++single_list.before_begin() == single_list.begin());
    std::cout << "Single element list: OK" << std::endl;

    // 大列表
    mys::forward_list<int> big_list;
    for (int i = 0; i < 1000; ++i) {
        big_list.push_front(i);
    }
    assert(big_list.size() == 1000);
    assert(big_list.front() == 999);
    std::cout << "Large list: OK" << std::endl;
}

int main() {
    std::cout << "Testing mys::forward_list implementation..." << std::endl;

    try {
        test_constructors_and_assignment();
        test_element_access();
        test_capacity_queries();
        test_modifiers();
        test_insert_and_erase();
        test_iterators();
        test_operations();
        test_comparison_operators();
        test_custom_types();
        test_edge_cases();

        std::cout << "\n=== ALL TESTS PASSED ===" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}