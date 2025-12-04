// test_list.cpp
#include "list.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace mys;

// Test fixture for basic list operations
class ListTest : public ::testing::Test {
protected:
    list<int> l;
};

// Test default constructor
TEST_F(ListTest, DefaultConstructor) {
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

// Test initializer list constructor
TEST_F(ListTest, InitializerListConstructor) {
    list<int> l{1, 2, 3, 4, 5};
    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 5);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 5);
}

// Test copy constructor
TEST_F(ListTest, CopyConstructor) {
    list<int> original{1, 2, 3};
    list<int> copy(original);

    EXPECT_EQ(copy.size(), 3);
    auto it1 = original.begin();
    auto it2 = copy.begin();
    while (it1 != original.end()) {
        EXPECT_EQ(*it1, *it2);
        ++it1;
        ++it2;
    }

    // Ensure deep copy
    copy.push_back(4);
    EXPECT_EQ(original.size(), 3);
    EXPECT_EQ(copy.size(), 4);
}

// Test move constructor
TEST_F(ListTest, MoveConstructor) {
    list<int> original{1, 2, 3};
    list<int> moved(std::move(original));

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 3);
    EXPECT_EQ(original.size(), 0); // 原对象应为空
}

// Test copy assignment
TEST_F(ListTest, CopyAssignment) {
    list<int> l1{1, 2, 3};
    list<int> l2;
    l2 = l1;

    EXPECT_EQ(l2.size(), 3);
    EXPECT_EQ(l2.front(), 1);
    EXPECT_EQ(l2.back(), 3);

    // Ensure deep copy
    l2.push_back(4);
    EXPECT_EQ(l1.size(), 3);
    EXPECT_EQ(l2.size(), 4);
}

// Test move assignment
TEST_F(ListTest, MoveAssignment) {
    list<int> l1{1, 2, 3};
    list<int> l2;
    l2 = std::move(l1);

    EXPECT_EQ(l2.size(), 3);
    EXPECT_EQ(l2.front(), 1);
    EXPECT_EQ(l2.back(), 3);
    EXPECT_EQ(l1.size(), 0);
}

// Test self-assignment
TEST_F(ListTest, SelfAssignment) {
    list<int> l{1, 2, 3};
    l = l;

    EXPECT_EQ(l.size(), 3);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 3);
}

// Test element access
TEST_F(ListTest, ElementAccess) {
    list<int> l{1, 2, 3};

    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 3);

    const auto &cl = l;
    EXPECT_EQ(cl.front(), 1);
    EXPECT_EQ(cl.back(), 3);
}

// Test element access on empty list
TEST_F(ListTest, ElementAccessEmpty) {
    list<int> l;
    EXPECT_THROW(l.front(), std::out_of_range);
    EXPECT_THROW(l.back(), std::out_of_range);
}

// Test push operations
TEST_F(ListTest, PushOperations) {
    list<int> l;

    l.push_back(1);
    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 1);

    l.push_front(0);
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.front(), 0);
    EXPECT_EQ(l.back(), 1);

    l.push_back(2);
    EXPECT_EQ(l.size(), 3);
    EXPECT_EQ(l.front(), 0);
    EXPECT_EQ(l.back(), 2);
}

// Test emplace operations
TEST_F(ListTest, EmplaceOperations) {
    list<std::string> l;
    l.emplace_back(3, 'a');  // Creates "aaa"
    l.emplace_front(2, 'b'); // Creates "bb"

    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.front(), "bb");
    EXPECT_EQ(l.back(), "aaa");
}

// Test pop operations
TEST_F(ListTest, PopOperations) {
    list<int> l{1, 2, 3};

    l.pop_back();
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.back(), 2);

    l.pop_front();
    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(l.front(), 2);

    l.pop_back();
    EXPECT_TRUE(l.empty());
}

// Test clear operation
TEST_F(ListTest, ClearOperation) {
    list<int> l{1, 2, 3, 4, 5};
    l.clear();

    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

// Test swap operation
TEST_F(ListTest, SwapOperation) {
    list<int> l1{1, 2, 3};
    list<int> l2{4, 5};

    l1.swap(l2);

    EXPECT_EQ(l1.size(), 2);
    EXPECT_EQ(l2.size(), 3);

    std::vector<int> v1(l1.begin(), l1.end());
    std::vector<int> v2(l2.begin(), l2.end());

    EXPECT_EQ(v1, std::vector<int>({4, 5}));
    EXPECT_EQ(v2, std::vector<int>({1, 2, 3}));
}

// Test iterator functionality
TEST_F(ListTest, IteratorOperations) {
    list<int> l{1, 2, 3, 4, 5};

    // Forward iteration
    std::vector<int> forward;
    for (auto it = l.begin(); it != l.end(); ++it) {
        forward.push_back(*it);
    }
    EXPECT_EQ(forward, std::vector<int>({1, 2, 3, 4, 5}));

    // Range-based for loop
    std::vector<int> range_based;
    for (const auto &item : l) {
        range_based.push_back(item);
    }
    EXPECT_EQ(range_based, std::vector<int>({1, 2, 3, 4, 5}));

    // Const iteration
    const auto &cl = l;
    std::vector<int> const_iter;
    for (auto it = cl.begin(); it != cl.end(); ++it) {
        const_iter.push_back(*it);
    }
    EXPECT_EQ(const_iter, std::vector<int>({1, 2, 3, 4, 5}));
}

// Test reverse iterator functionality
TEST_F(ListTest, ReverseIteratorOperations) {
    list<int> l{1, 2, 3, 4, 5};

    std::vector<int> reverse;
    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        reverse.push_back(*it);
    }
    EXPECT_EQ(reverse, std::vector<int>({5, 4, 3, 2, 1}));

    // Const reverse iteration
    const auto &cl = l;
    std::vector<int> const_reverse;
    for (auto it = cl.rbegin(); it != cl.rend(); ++it) {
        const_reverse.push_back(*it);
    }
    EXPECT_EQ(const_reverse, std::vector<int>({5, 4, 3, 2, 1}));
}

// Test insert operations
TEST_F(ListTest, InsertOperations) {
    list<int> l{1, 3, 4};

    // Insert at beginning
    auto it = l.insert(l.begin(), 0);
    EXPECT_EQ(*it, 0);
    EXPECT_EQ(l.size(), 4);

    std::vector<int> expected{0, 1, 3, 4};
    std::vector<int> actual(l.begin(), l.end());
    EXPECT_EQ(actual, expected);

    // Insert at end
    it = l.insert(l.end(), 5);
    EXPECT_EQ(*it, 5);
    EXPECT_EQ(l.size(), 5);

    std::vector<int> expected2{0, 1, 3, 4, 5};
    std::vector<int> actual2(l.begin(), l.end());
    EXPECT_EQ(actual2, expected2);

    // Insert in middle
    auto pos = l.begin();
    std::advance(pos, 2);
    it = l.insert(pos, 2);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(l.size(), 6);

    std::vector<int> expected3{0, 1, 2, 3, 4, 5};
    std::vector<int> actual3(l.begin(), l.end());
    EXPECT_EQ(actual3, expected3);
}

// Test emplace operation
TEST_F(ListTest, EmplaceOperation) {
    list<std::string> l{"aa", "cc"};

    auto pos = l.begin();
    ++pos;
    auto it = l.emplace(pos, 3, 'b'); // Creates "bbb"

    EXPECT_EQ(*it, "bbb");
    EXPECT_EQ(l.size(), 3);

    std::vector<std::string> expected{"aa", "bbb", "cc"};
    std::vector<std::string> actual(l.begin(), l.end());
    EXPECT_EQ(actual, expected);
}

// Test erase operations
TEST_F(ListTest, EraseOperations) {
    list<int> l{0, 1, 2, 3, 4, 5};

    // Erase single element
    auto pos = l.begin();
    std::advance(pos, 2);
    auto it = l.erase(pos); // Erase element 2
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(l.size(), 5);

    std::vector<int> expected{0, 1, 3, 4, 5};
    std::vector<int> actual(l.begin(), l.end());
    EXPECT_EQ(actual, expected);

    // Erase range
    auto first = l.begin();
    auto last = l.begin();
    std::advance(first, 1);
    std::advance(last, 3);
    it = l.erase(first, last); // Erase elements 1 and 3
    EXPECT_EQ(*it, 4);
    EXPECT_EQ(l.size(), 3);

    std::vector<int> expected2{0, 4, 5};
    std::vector<int> actual2(l.begin(), l.end());
    EXPECT_EQ(actual2, expected2);
}

// Test comparison operators
TEST_F(ListTest, ComparisonOperators) {
    list<int> l1{1, 2, 3};
    list<int> l2{1, 2, 3};
    list<int> l3{1, 2, 4};
    list<int> l4{1, 2};
    list<int> l5{1, 2, 3, 4};

    // Equality
    EXPECT_TRUE(l1 == l2);
    EXPECT_FALSE(l1 == l3);

    // Inequality
    EXPECT_TRUE(l1 != l3);
    EXPECT_FALSE(l1 != l2);

    // Less than
    EXPECT_TRUE(l1 < l3);
    EXPECT_TRUE(l4 < l1);
    EXPECT_FALSE(l1 < l2);
    EXPECT_FALSE(l3 < l1);

    // Less or equal
    EXPECT_TRUE(l1 <= l2);
    EXPECT_TRUE(l1 <= l3);
    EXPECT_TRUE(l4 <= l1);

    // Greater than
    EXPECT_TRUE(l3 > l1);
    EXPECT_TRUE(l1 > l4);
    EXPECT_FALSE(l1 > l2);
    EXPECT_FALSE(l1 > l3);

    // Greater or equal
    EXPECT_TRUE(l1 >= l2);
    EXPECT_TRUE(l3 >= l1);
    EXPECT_TRUE(l1 >= l4);
}

// Test with different types
TEST_F(ListTest, DifferentTypes) {
    // String list
    list<std::string> string_list{"hello", "world"};
    EXPECT_EQ(string_list.size(), 2);
    EXPECT_EQ(string_list.front(), "hello");
    EXPECT_EQ(string_list.back(), "world");

    // List of lists
    list<list<int>> list_of_lists{{1, 2}, {3, 4}};
    EXPECT_EQ(list_of_lists.size(), 2);
    EXPECT_EQ(list_of_lists.front().size(), 2);
    EXPECT_EQ(list_of_lists.back().front(), 3);
}

// Test move-only types
TEST_F(ListTest, MoveOnlyTypes) {
    list<std::unique_ptr<int>> l;
    l.push_back(std::make_unique<int>(42));
    l.emplace_back(new int(100));

    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(*l.front(), 42);
    EXPECT_EQ(*l.back(), 100);

    // Test move operations
    auto ptr = std::move(l.front());
    EXPECT_EQ(*ptr, 42);
    EXPECT_EQ(l.front(), nullptr);
}

// Main function
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}