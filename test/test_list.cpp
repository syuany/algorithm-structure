#include "list.h"
#include <iostream>
#include <string>
#include <cassert>
#include <stdexcept>

// 测试用的自定义类型
struct TestObject {
    int value;
    static int constructions;
    static int destructions;
    static int copies;
    static int moves;

    TestObject(int v = 0) : value(v) { ++constructions; }

    TestObject(const TestObject &other) : value(other.value) { ++copies; }

    TestObject(TestObject &&other) noexcept : value(other.value) {
        ++moves;
        other.value = -1;
    }

    TestObject &operator=(const TestObject &other) {
        if (this != &other) {
            value = other.value;
            ++copies;
        }
        return *this;
    }

    TestObject &operator=(TestObject &&other) noexcept {
        if (this != &other) {
            value = other.value;
            ++moves;
            other.value = -1;
        }
        return *this;
    }

    ~TestObject() { ++destructions; }

    bool operator==(const TestObject &other) const { return value == other.value; }

    auto operator<=>(const TestObject &other) const = default;
};

int TestObject::constructions = 0;
int TestObject::destructions = 0;
int TestObject::copies = 0;
int TestObject::moves = 0;

void reset_counters() {
    TestObject::constructions = 0;
    TestObject::destructions = 0;
    TestObject::copies = 0;
    TestObject::moves = 0;
}

// 测试默认构造函数
void test_default_constructor() {
    std::cout << "Testing default constructor...\n";
    mys::list<int> l;
    assert(l.empty());
    assert(l.size() == 0);
    std::cout << "Default constructor test passed.\n";
}

// 测试初始化列表构造函数
void test_initializer_list_constructor() {
    std::cout << "Testing initializer list constructor...\n";
    mys::list<int> l{1, 2, 3, 4, 5};
    assert(!l.empty());
    assert(l.size() == 5);
    assert(l.front() == 1);
    assert(l.back() == 5);

    // 空初始化列表
    mys::list<int> empty{};
    assert(empty.empty());
    assert(empty.size() == 0);
    std::cout << "Initializer list constructor test passed.\n";
}

// 测试拷贝构造函数
void test_copy_constructor() {
    std::cout << "Testing copy constructor...\n";
    mys::list<int> original{1, 2, 3};
    mys::list<int> copy(original);

    assert(copy.size() == 3);
    assert(copy.front() == 1);
    assert(copy.back() == 3);

    // 修改原列表不应影响拷贝
    original.push_back(4);
    assert(original.size() == 4);
    assert(copy.size() == 3);
    std::cout << "Copy constructor test passed.\n";
}

// 测试移动构造函数
void test_move_constructor() {
    std::cout << "Testing move constructor...\n";
    reset_counters();

    mys::list<TestObject> original;
    original.emplace_back(1);
    original.emplace_back(2);
    original.emplace_back(3);

    int constructions_before_move = TestObject::constructions;
    int moves_before_move = TestObject::moves;

    mys::list<TestObject> moved(std::move(original));

    assert(moved.size() == 3);
    assert(moved.front().value == 1);
    assert(moved.back().value == 3);

    // 移动后原对象应该是空的
    assert(original.empty());
    assert(original.size() == 0);
    std::cout << "Move constructor test passed.\n";
}

// 测试拷贝赋值操作符
void test_copy_assignment() {
    std::cout << "Testing copy assignment...\n";
    mys::list<int> l1{1, 2, 3};
    mys::list<int> l2{4, 5, 6, 7};

    l2 = l1;
    assert(l2.size() == 3);
    assert(l2.front() == 1);
    assert(l2.back() == 3);

    // 自赋值测试
    l1 = l1;
    assert(l1.size() == 3);
    std::cout << "Copy assignment test passed.\n";
}

// 测试移动赋值操作符
void test_move_assignment() {
    std::cout << "Testing move assignment...\n";
    mys::list<int> l1{1, 2, 3};
    mys::list<int> l2{4, 5, 6, 7};

    l2 = std::move(l1);
    assert(l2.size() == 3);
    assert(l2.front() == 1);
    assert(l2.back() == 3);
    assert(l1.empty()); // 原对象应为空
    std::cout << "Move assignment test passed.\n";
}

// 测试元素访问方法
void test_element_access() {
    std::cout << "Testing element access...\n";
    mys::list<int> l{1, 2, 3, 4, 5};

    // 测试 front 和 back
    assert(l.front() == 1);
    assert(l.back() == 5);

    // 测试 const 版本
    const mys::list<int> &cl = l;
    assert(cl.front() == 1);
    assert(cl.back() == 5);

    // 修改测试
    l.front() = 10;
    l.back() = 50;
    assert(l.front() == 10);
    assert(l.back() == 50);
    std::cout << "Element access test passed.\n";
}

// 测试容量查询
void test_capacity() {
    std::cout << "Testing capacity...\n";
    mys::list<int> empty_list;
    mys::list<int> filled_list{1, 2, 3};

    // 空列表测试
    assert(empty_list.empty());
    assert(empty_list.size() == 0);

    // 非空列表测试
    assert(!filled_list.empty());
    assert(filled_list.size() == 3);
    std::cout << "Capacity test passed.\n";
}

// 测试修改器 - push/pop 操作
void test_push_pop_operations() {
    std::cout << "Testing push/pop operations...\n";
    mys::list<int> l;

    // 测试 push_back
    l.push_back(1);
    assert(l.size() == 1);
    assert(l.back() == 1);

    l.push_back(2);
    assert(l.size() == 2);
    assert(l.back() == 2);

    // 测试 push_front
    l.push_front(0);
    assert(l.size() == 3);
    assert(l.front() == 0);

    // 测试 pop_back
    l.pop_back();
    assert(l.size() == 2);
    assert(l.back() == 1);

    // 测试 pop_front
    l.pop_front();
    assert(l.size() == 1);
    assert(l.front() == 1);

    // 测试清空所有元素
    l.pop_front();
    assert(l.empty());

    // 在空列表上执行 pop 操作（边界情况）
    // 注意：根据标准库行为，对空容器调用 pop 是未定义行为
    // 我们的测试假设实现能妥善处理这种情况
    std::cout << "Push/pop operations test passed.\n";
}

// 测试 emplace 操作
void test_emplace_operations() {
    std::cout << "Testing emplace operations...\n";
    reset_counters();

    mys::list<TestObject> l;

    // 测试 emplace_back
    l.emplace_back(42);
    assert(l.size() == 1);
    assert(l.front().value == 42);

    // 测试 emplace_front
    l.emplace_front(10);
    assert(l.size() == 2);
    assert(l.front().value == 10);

    assert(TestObject::constructions > 0);
    std::cout << "Emplace operations test passed.\n";
}

// 修改 test_iterators 函数以帮助定位问题
void test_iterators() {
    std::cout << "Testing iterators...\n";
    mys::list<int> l{1, 2, 3, 4, 5};

    // std::cout << "Testing forward iterator...\n";
    // 正向迭代器
    int expected = 1;
    for (auto it = l.begin(); it != l.end(); ++it) {
        // std::cout << "Accessing element: " << *it << std::endl;
        assert(*it == expected++);
    }

    // std::cout << "Testing range-based for loop...\n";
    // 范围for循环
    expected = 1;
    for (const auto &value : l) {
        // std::cout << "Range-for element: " << value << std::endl;
        assert(value == expected++);
    }

    // std::cout << "Testing reverse iterator...\n";
    // 反向迭代器
    expected = 5;
    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        // std::cout << "Reverse element: " << *it << std::endl;
        assert(*it == expected--);
    }

    // std::cout << "Testing const iterator...\n";
    // const迭代器
    const mys::list<int> &cl = l;
    expected = 1;
    for (auto it = cl.cbegin(); it != cl.cend(); ++it) {
        // std::cout << "Const iterator element: " << *it << std::endl;
        assert(*it == expected++);
    }

    // std::cout << "Testing iterator equality...\n";
    // 测试迭代器相等性
    auto it1 = l.begin();
    auto it2 = l.begin();
    assert(it1 == it2);

    ++it1;
    assert(it1 != it2);
    std::cout << "Iterators test passed.\n";
}

// 测试插入操作
void test_insert_operations() {
    std::cout << "Testing insert operations...\n";
    mys::list<int> l{1, 3, 5};

    // 在中间插入
    auto it = l.insert(++l.begin(), 2);
    assert(*it == 2);
    assert(l.size() == 4);

    // 在开头插入
    it = l.insert(l.begin(), 0);
    assert(*it == 0);
    assert(l.front() == 0);

    // 在末尾插入
    it = l.insert(l.end(), 6);
    assert(*it == 6);
    assert(l.back() == 6);

    // 验证顺序
    int expected[] = {0, 1, 2, 3, 5, 6};
    int i = 0;
    for (const auto &value : l) {
        assert(value == expected[i++]);
    }
    std::cout << "Insert operations test passed.\n";
}

// 测试擦除操作
void test_erase_operations() {
    std::cout << "Testing erase operations...\n";
    mys::list<int> l{0, 1, 2, 3, 4, 5};

    // 擦除单个元素 (擦除元素1)
    auto it = l.erase(++l.begin());
    assert(*it == 2); // 返回指向下一个元素(2)的迭代器
    assert(l.size() == 5);
    // 当前列表状态: {0, 2, 3, 4, 5}

    // 擦除范围 (从元素2到元素4，不包括元素5)
    auto first = ++l.begin();  // 指向元素2
    auto last = --l.end();     // 指向元素5 (注意: 这里是元素5，不是4)
    it = l.erase(first, last); // 擦除[2,5) 即元素2,3,4
    assert(*it == 5);          // 返回指向被删除元素后一个(5)的迭代器
    assert(l.size() == 2);

    // 验证剩余元素
    int expected[] = {0, 5};
    int i = 0;
    for (const auto &value : l) {
        assert(value == expected[i++]);
    }

    // 擦除所有元素
    l.erase(l.begin(), l.end());
    assert(l.empty());
    std::cout << "Erase operations test passed.\n";
}

// 测试清除操作
void test_clear() {
    std::cout << "Testing clear...\n";
    mys::list<int> l{1, 2, 3, 4, 5};
    assert(l.size() == 5);

    l.clear();
    assert(l.empty());
    assert(l.size() == 0);

    // 再次清除空列表
    l.clear();
    assert(l.empty());
    std::cout << "Clear test passed.\n";
}

// 测试交换操作
void test_swap() {
    std::cout << "Testing swap...\n";
    mys::list<int> l1{1, 2, 3};
    mys::list<int> l2{4, 5, 6, 7};

    l1.swap(l2);

    assert(l1.size() == 4);
    assert(l2.size() == 3);
    assert(l1.front() == 4);
    assert(l2.front() == 1);

    // 使用ADL交换
    using std::swap;
    swap(l1, l2);

    assert(l1.size() == 3);
    assert(l2.size() == 4);
    assert(l1.front() == 1);
    assert(l2.front() == 4);
    std::cout << "Swap test passed.\n";
}

// 测试比较操作
void test_comparison() {
    std::cout << "Testing comparison...\n";
    mys::list<int> l1{1, 2, 3};
    mys::list<int> l2{1, 2, 3};
    mys::list<int> l3{1, 2, 4};
    mys::list<int> l4{1, 2};

    // 相等性测试
    assert(l1 == l2);
    assert(!(l1 != l2));
    assert(l1 != l3);

    // 三路比较测试
    assert((l1 <=> l2) == std::strong_ordering::equal);
    assert((l1 <=> l3) == std::strong_ordering::less);
    assert((l3 <=> l1) == std::strong_ordering::greater);
    assert((l1 <=> l4) == std::strong_ordering::greater);
    std::cout << "Comparison test passed.\n";
}

// 边界情况测试
void test_edge_cases() {
    std::cout << "Testing edge cases...\n";

    // 1. 空列表上的各种操作
    mys::list<int> empty_list;
    assert(empty_list.empty());
    assert(empty_list.size() == 0);

    // 2. 单元素列表
    mys::list<int> single_elem{42};
    assert(single_elem.size() == 1);
    assert(single_elem.front() == 42);
    assert(single_elem.back() == 42);

    // 3. 两元素列表（测试前后连接）
    mys::list<int> two_elem{1, 2};
    assert(two_elem.size() == 2);
    assert(two_elem.front() == 1);
    assert(two_elem.back() == 2);

    // 4. 大量元素操作
    mys::list<int> large_list;
    for (int i = 0; i < 1000; ++i) {
        large_list.push_back(i);
    }
    assert(large_list.size() == 1000);
    assert(large_list.front() == 0);
    assert(large_list.back() == 999);

    // 5. 迭代器边界情况
    auto it = empty_list.begin();
    assert(it == empty_list.end());

    // 6. 在空列表上插入
    auto insert_it = empty_list.insert(empty_list.begin(), 1);
    assert(empty_list.size() == 1);
    assert(*insert_it == 1);

    std::cout << "Edge cases test passed.\n";
}

// 测试资源管理
void test_resource_management() {
    std::cout << "Testing resource management...\n";
    reset_counters();

    {
        mys::list<TestObject> l;
        for (int i = 0; i < 10; ++i) {
            l.emplace_back(i);
        }
        assert(l.size() == 10);
    } // 列表析构

    // 所有对象都应该被正确析构
    assert(TestObject::constructions == TestObject::destructions);
    std::cout << "Resource management test passed.\n";
}

int main() {
    try {
        std::cout << "Starting comprehensive tests for mys::list...\n\n";

        test_default_constructor();
        test_initializer_list_constructor();
        test_copy_constructor();
        test_move_constructor();
        test_copy_assignment();
        test_move_assignment();
        test_element_access();
        test_capacity();
        test_push_pop_operations();
        test_emplace_operations();
        test_iterators();
        test_insert_operations();
        test_erase_operations();
        test_clear();
        test_swap();
        test_comparison();
        test_edge_cases();
        test_resource_management();

        std::cout << "\nAll tests passed successfully!\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}