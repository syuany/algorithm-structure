// test_forward_list.cpp
#include "forward_list.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace mys;

// Test fixture for forward_list tests
class ForwardListTest : public ::testing::Test {
protected:
    forward_list<int> fl;
};

// Test default constructor
TEST_F(ForwardListTest, DefaultConstructor) {
    EXPECT_TRUE(fl.empty());
    EXPECT_EQ(fl.size(), 0);
}

// Test initializer list constructor
TEST_F(ForwardListTest, InitializerListConstructor) {
    forward_list<int> fl{1, 2, 3, 4, 5};
    EXPECT_FALSE(fl.empty());
    EXPECT_EQ(fl.size(), 5);
    EXPECT_EQ(fl.front(), 1);
}

// Test copy constructor
TEST_F(ForwardListTest, CopyConstructor) {
    forward_list<int> original{1, 2, 3};
    forward_list<int> copy(original);

    EXPECT_EQ(copy.size(), 3);
    auto it1 = original.begin();
    auto it2 = copy.begin();
    while (it1 != original.end()) {
        EXPECT_EQ(*it1, *it2);
        ++it1;
        ++it2;
    }

    // Ensure deep copy
    copy.push_front(0);
    EXPECT_EQ(original.size(), 3);
    EXPECT_EQ(copy.size(), 4);
}

// Test move constructor
TEST_F(ForwardListTest, MoveConstructor) {
    forward_list<int> original{1, 2, 3};
    forward_list<int> moved(std::move(original));

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(original.size(), 0);
}

// Test copy assignment
TEST_F(ForwardListTest, CopyAssignment) {
    forward_list<int> fl1{1, 2, 3};
    forward_list<int> fl2;
    fl2 = fl1;

    EXPECT_EQ(fl2.size(), 3);
    EXPECT_EQ(fl2.front(), 1);

    // Ensure deep copy
    fl2.push_front(0);
    EXPECT_EQ(fl1.size(), 3);
    EXPECT_EQ(fl2.size(), 4);
}

// Test move assignment
TEST_F(ForwardListTest, MoveAssignment) {
    forward_list<int> fl1{1, 2, 3};
    forward_list<int> fl2;
    fl2 = std::move(fl1);

    EXPECT_EQ(fl2.size(), 3);
    EXPECT_EQ(fl2.front(), 1);
    EXPECT_EQ(fl1.size(), 0);
}

// Test element access
TEST_F(ForwardListTest, ElementAccess) {
    forward_list<int> fl{1, 2, 3};
    EXPECT_EQ(fl.front(), 1);

    const auto &cfl = fl;
    EXPECT_EQ(cfl.front(), 1);
}

// Test element access on empty list
// TEST_F(ForwardListTest, ElementAccessEmpty) {
//     forward_list<int> fl;
//     EXPECT_THROW(fl.front(), std::out_of_range);
// }

// Test push operations
TEST_F(ForwardListTest, PushOperations) {
    forward_list<int> fl;

    fl.push_front(1);
    EXPECT_EQ(fl.size(), 1);
    EXPECT_EQ(fl.front(), 1);

    fl.push_front(0);
    EXPECT_EQ(fl.size(), 2);
    EXPECT_EQ(fl.front(), 0);
}

// Test emplace operations
TEST_F(ForwardListTest, EmplaceOperations) {
    forward_list<std::string> fl;
    fl.emplace_front(3, 'a'); // Creates "aaa"
    fl.emplace_front(2, 'b'); // Creates "bb"

    EXPECT_EQ(fl.size(), 2);
    EXPECT_EQ(fl.front(), "bb");
}

// Test pop operations
TEST_F(ForwardListTest, PopOperations) {
    forward_list<int> fl{1, 2, 3};

    fl.pop_front();
    EXPECT_EQ(fl.size(), 2);
    EXPECT_EQ(fl.front(), 2);

    fl.pop_front();
    EXPECT_EQ(fl.size(), 1);
    EXPECT_EQ(fl.front(), 3);
}

// Test clear operation
TEST_F(ForwardListTest, ClearOperation) {
    forward_list<int> fl{1, 2, 3, 4, 5};
    fl.clear();

    EXPECT_TRUE(fl.empty());
    EXPECT_EQ(fl.size(), 0);
}

// Test swap operation
TEST_F(ForwardListTest, SwapOperation) {
    forward_list<int> fl1{1, 2, 3};
    forward_list<int> fl2{4, 5};

    fl1.swap(fl2);

    EXPECT_EQ(fl1.size(), 2);
    EXPECT_EQ(fl2.size(), 3);

    std::vector<int> v1(fl1.begin(), fl1.end());
    std::vector<int> v2(fl2.begin(), fl2.end());

    EXPECT_EQ(v1, std::vector<int>({4, 5}));
    EXPECT_EQ(v2, std::vector<int>({1, 2, 3}));
}

// Test iterator functionality
TEST_F(ForwardListTest, IteratorOperations) {
    forward_list<int> fl{1, 2, 3, 4, 5};

    // Forward iteration
    std::vector<int> forward;
    for (auto it = fl.begin(); it != fl.end(); ++it) {
        forward.push_back(*it);
    }
    EXPECT_EQ(forward, std::vector<int>({1, 2, 3, 4, 5}));

    // Range-based for loop
    std::vector<int> range_based;
    for (const auto &item : fl) {
        range_based.push_back(item);
    }
    EXPECT_EQ(range_based, std::vector<int>({1, 2, 3, 4, 5}));

    // Const iteration
    const auto &cfl = fl;
    std::vector<int> const_iter;
    for (auto it = cfl.begin(); it != cfl.end(); ++it) {
        const_iter.push_back(*it);
    }
    EXPECT_EQ(const_iter, std::vector<int>({1, 2, 3, 4, 5}));
}

// Test before_begin iterator
TEST_F(ForwardListTest, BeforeBeginIterator) {
    forward_list<int> fl{1, 2, 3};

    auto it = fl.before_begin();
    ++it;
    EXPECT_EQ(*it, 1);

    const auto &cfl = fl;
    auto cit = cfl.before_begin();
    ++cit;
    EXPECT_EQ(*cit, 1);
}

// Test insert_after operations
TEST_F(ForwardListTest, InsertAfterOperations) {
    forward_list<int> fl{1, 3, 4};

    // Insert after begin (at position 0)
    auto it = fl.insert_after(fl.before_begin(), 0);
    EXPECT_EQ(*it, 0);
    EXPECT_EQ(fl.size(), 4);

    std::vector<int> expected{0, 1, 3, 4};
    std::vector<int> actual(fl.begin(), fl.end());
    EXPECT_EQ(actual, expected);

    // Insert in middle
    auto pos = fl.begin();
    ++pos; // Points to 1
    it = fl.insert_after(pos, 2);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(fl.size(), 5);

    std::vector<int> expected2{0, 1, 2, 3, 4};
    std::vector<int> actual2(fl.begin(), fl.end());
    EXPECT_EQ(actual2, expected2);
}

// Test emplace_after operation
TEST_F(ForwardListTest, EmplaceAfterOperation) {
    forward_list<std::string> fl{"aa", "cc"};

    auto pos = fl.begin();
    auto it = fl.emplace_after(pos, 3, 'b'); // Creates "bbb"

    EXPECT_EQ(*it, "bbb");
    EXPECT_EQ(fl.size(), 3);

    std::vector<std::string> expected{"aa", "bbb", "cc"};
    std::vector<std::string> actual(fl.begin(), fl.end());
    EXPECT_EQ(actual, expected);
}

// Test erase_after operations
TEST_F(ForwardListTest, EraseAfterOperations) {
    forward_list<int> fl{0, 1, 2, 3, 4, 5};

    // Erase single element
    auto pos = fl.before_begin();
    auto it = fl.erase_after(pos); // Erase element 0
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(fl.size(), 5);

    std::vector<int> expected{1, 2, 3, 4, 5};
    std::vector<int> actual(fl.begin(), fl.end());
    EXPECT_EQ(actual, expected);

    // Erase range
    auto first = fl.before_begin();
    auto last = fl.begin();
    std::advance(last, 2);            // Points to element 3
    it = fl.erase_after(first, last); // Erase elements 1 and 2
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(fl.size(), 3);

    std::vector<int> expected2{3, 4, 5};
    std::vector<int> actual2(fl.begin(), fl.end());
    EXPECT_EQ(actual2, expected2);
}

// Test splice_after operations
TEST_F(ForwardListTest, SpliceAfterOperations) {
    forward_list<int> fl1{1, 2, 3};
    forward_list<int> fl2{4, 5};

    // Splice entire list
    fl1.splice_after(fl1.before_begin(), fl2);

    EXPECT_EQ(fl1.size(), 5);
    EXPECT_EQ(fl2.size(), 0);

    std::vector<int> expected{4, 5, 1, 2, 3};
    std::vector<int> actual(fl1.begin(), fl1.end());
    EXPECT_EQ(actual, expected);

    // Splice single element
    forward_list<int> fl3{6, 7, 8};
    forward_list<int> fl4{9, 10};

    auto pos = fl3.begin();
    fl3.splice_after(pos, fl4, fl4.before_begin());

    std::vector<int> expected3{6, 9, 7, 8};
    std::vector<int> actual3(fl3.begin(), fl3.end());
    EXPECT_EQ(actual3, expected3);

    std::vector<int> expected4{10};
    std::vector<int> actual4(fl4.begin(), fl4.end());
    EXPECT_EQ(actual4, expected4);
}

// Test remove operations
TEST_F(ForwardListTest, RemoveOperations) {
    forward_list<int> fl{1, 2, 3, 2, 4, 2, 5};

    fl.remove(2);
    EXPECT_EQ(fl.size(), 4);

    std::vector<int> expected{1, 3, 4, 5};
    std::vector<int> actual(fl.begin(), fl.end());
    EXPECT_EQ(actual, expected);

    // Test remove_if
    forward_list<int> fl2{1, 2, 3, 4, 5, 6};
    fl2.remove_if([](int x) { return x % 2 == 0; }); // Remove even numbers

    std::vector<int> expected2{1, 3, 5};
    std::vector<int> actual2(fl2.begin(), fl2.end());
    EXPECT_EQ(actual2, expected2);
}

// Test unique operations
TEST_F(ForwardListTest, UniqueOperations) {
    forward_list<int> fl{1, 1, 2, 2, 3, 3, 3, 4, 5, 5};

    fl.unique();
    EXPECT_EQ(fl.size(), 5);

    std::vector<int> expected{1, 2, 3, 4, 5};
    std::vector<int> actual(fl.begin(), fl.end());
    EXPECT_EQ(actual, expected);

    // Test unique with custom predicate
    forward_list<int> fl2{1, 2, 4, 5, 7, 8};
    fl2.unique([](int a, int b) { return (b - a) == 1; }); // Remove consecutive numbers

    std::vector<int> expected2{1, 4, 7};
    std::vector<int> actual2(fl2.begin(), fl2.end());
    EXPECT_EQ(actual2, expected2);
}

// Test reverse operation
TEST_F(ForwardListTest, ReverseOperation) {
    forward_list<int> fl{1, 2, 3, 4, 5};
    fl.reverse();

    std::vector<int> expected{5, 4, 3, 2, 1};
    std::vector<int> actual(fl.begin(), fl.end());
    EXPECT_EQ(actual, expected);
}

// Test comparison operators
TEST_F(ForwardListTest, ComparisonOperators) {
    forward_list<int> fl1{1, 2, 3};
    forward_list<int> fl2{1, 2, 3};
    forward_list<int> fl3{1, 2, 4};
    forward_list<int> fl4{1, 2};
    forward_list<int> fl5{1, 2, 3, 4};

    // Equality
    EXPECT_TRUE(fl1 == fl2);
    EXPECT_FALSE(fl1 == fl3);

    // Inequality
    EXPECT_TRUE(fl1 != fl3);
    EXPECT_FALSE(fl1 != fl2);

    // Less than
    EXPECT_TRUE(fl1 < fl3);
    EXPECT_TRUE(fl4 < fl1);
    EXPECT_FALSE(fl1 < fl2);
    EXPECT_FALSE(fl3 < fl1);

    // Less or equal
    EXPECT_TRUE(fl1 <= fl2);
    EXPECT_TRUE(fl1 <= fl3);
    EXPECT_TRUE(fl4 <= fl1);

    // Greater than
    EXPECT_TRUE(fl3 > fl1);
    EXPECT_TRUE(fl1 > fl4);
    EXPECT_FALSE(fl1 > fl2);
    EXPECT_FALSE(fl1 > fl3);

    // Greater or equal
    EXPECT_TRUE(fl1 >= fl2);
    EXPECT_TRUE(fl3 >= fl1);
    EXPECT_TRUE(fl1 >= fl4);
}

// Test with different types
TEST_F(ForwardListTest, DifferentTypes) {
    // String list
    forward_list<std::string> string_list{"hello", "world"};
    EXPECT_EQ(string_list.size(), 2);
    EXPECT_EQ(string_list.front(), "hello");

    // List of lists
    forward_list<forward_list<int>> list_of_lists{{1, 2}, {3, 4}};
    EXPECT_EQ(list_of_lists.size(), 2);
    EXPECT_EQ(list_of_lists.front().size(), 2);
    auto it = list_of_lists.front().begin();
    EXPECT_EQ(*it, 1);
}

// Test move-only types
TEST_F(ForwardListTest, MoveOnlyTypes) {
    forward_list<std::unique_ptr<int>> fl;
    fl.push_front(std::make_unique<int>(42));
    fl.emplace_front(new int(100));

    EXPECT_EQ(fl.size(), 2);
    EXPECT_EQ(*fl.front(), 100);

    // Test move operations
    auto ptr = std::move(fl.front());
    EXPECT_EQ(*ptr, 100);
    EXPECT_EQ(fl.front(), nullptr);
}

// Main function
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}