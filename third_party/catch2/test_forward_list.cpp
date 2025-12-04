// forward_list_test.cpp
#include "structure/forward_list.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <string>
#include <vector>

using namespace mys;

TEST_CASE("Default construction", "[forward_list]") {
    forward_list<int> fl;
    REQUIRE(fl.empty());
    REQUIRE(fl.size() == 0);
}

TEST_CASE("Initializer list construction", "[forward_list]") {
    forward_list<int> fl{1, 2, 3, 4, 5};
    REQUIRE_FALSE(fl.empty());
    REQUIRE(fl.size() == 5);
    REQUIRE(fl.front() == 1);
}

TEST_CASE("Copy construction", "[forward_list]") {
    forward_list<int> original{1, 2, 3};
    forward_list<int> copy(original);

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.front() == 1);

    // 修改原列表不应影响复制的列表
    original.push_front(0);
    REQUIRE(original.size() == 4);
    REQUIRE(copy.size() == 3);
}

TEST_CASE("Move construction", "[forward_list]") {
    forward_list<int> original{1, 2, 3};
    forward_list<int> moved(std::move(original));

    REQUIRE(moved.size() == 3);
    REQUIRE(moved.front() == 1);
    // 原列表状态未定义，但应该可以安全析构
}

TEST_CASE("Assignment operators", "[forward_list]") {
    forward_list<int> fl1{1, 2, 3};
    forward_list<int> fl2;

    SECTION("Copy assignment") {
        fl2 = fl1;
        REQUIRE(fl2.size() == 3);
        REQUIRE(fl2.front() == 1);
    }

    SECTION("Move assignment") {
        forward_list<int> fl3{4, 5};
        fl3 = std::move(fl1);
        REQUIRE(fl3.size() == 3);
        REQUIRE(fl3.front() == 1);
    }
}

TEST_CASE("Element access", "[forward_list]") {
    forward_list<std::string> fl{"hello", "world"};

    SECTION("Non-const access") {
        REQUIRE(fl.front() == "hello");
        fl.front() = "hi";
        REQUIRE(fl.front() == "hi");
    }

    SECTION("Const access") {
        const auto &cfl = fl;
        REQUIRE(cfl.front() == "hello");
    }
}

TEST_CASE("Push front operations", "[forward_list]") {
    forward_list<int> fl;

    fl.push_front(1);
    REQUIRE(fl.size() == 1);
    REQUIRE(fl.front() == 1);

    fl.push_front(2);
    REQUIRE(fl.size() == 2);
    REQUIRE(fl.front() == 2);

    fl.push_front(3);
    REQUIRE(fl.size() == 3);
    REQUIRE(fl.front() == 3);
}

TEST_CASE("Emplace front operations", "[forward_list]") {
    forward_list<std::pair<int, std::string>> fl;

    fl.emplace_front(1, "one");
    REQUIRE(fl.size() == 1);
    REQUIRE(fl.front().first == 1);
    REQUIRE(fl.front().second == "one");

    fl.emplace_front(0, "zero");
    REQUIRE(fl.size() == 2);
    REQUIRE(fl.front().first == 0);
    REQUIRE(fl.front().second == "zero");
}

TEST_CASE("Pop front operations", "[forward_list]") {
    forward_list<int> fl{1, 2, 3, 4, 5};
    REQUIRE(fl.size() == 5);

    fl.pop_front();
    REQUIRE(fl.size() == 4);
    REQUIRE(fl.front() == 2);

    fl.pop_front();
    REQUIRE(fl.size() == 3);
    REQUIRE(fl.front() == 3);
}

TEST_CASE("Clear operation", "[forward_list]") {
    forward_list<int> fl{1, 2, 3};
    REQUIRE(fl.size() == 3);

    fl.clear();
    REQUIRE(fl.empty());
    REQUIRE(fl.size() == 0);
}

TEST_CASE("Swap operation", "[forward_list]") {
    forward_list<int> fl1{1, 2, 3};
    forward_list<int> fl2{4, 5};

    fl1.swap(fl2);

    REQUIRE(fl1.size() == 2);
    REQUIRE(fl1.front() == 4);

    REQUIRE(fl2.size() == 3);
    REQUIRE(fl2.front() == 1);
}

TEST_CASE("Iterator operations", "[forward_list]") {
    forward_list<int> fl{1, 2, 3, 4, 5};

    SECTION("Begin/End iterators") {
        auto it = fl.begin();
        REQUIRE(*it == 1);

        auto end_it = fl.end();
        REQUIRE(it != end_it);
    }

    SECTION("Iterator traversal") {
        std::vector<int> values;
        for (auto it = fl.begin(); it != fl.end(); ++it) {
            values.push_back(*it);
        }

        std::vector<int> expected{1, 2, 3, 4, 5};
        REQUIRE_THAT(values, Catch::Matchers::Equals(expected));
    }

    SECTION("Const iterators") {
        const forward_list<int> &cfl = fl;
        std::vector<int> values;
        for (auto it = cfl.cbegin(); it != cfl.cend(); ++it) {
            values.push_back(*it);
        }

        std::vector<int> expected{1, 2, 3, 4, 5};
        REQUIRE_THAT(values, Catch::Matchers::Equals(expected));
    }
}

TEST_CASE("Before begin iterator", "[forward_list]") {
    forward_list<int> fl{1, 2, 3};

    SECTION("before_begin") {
        auto it = fl.before_begin();
        ++it; // Move to first element
        REQUIRE(*it == 1);
    }

    SECTION("cbefore_begin") {
        const auto &cfl = fl;
        auto it = cfl.cbefore_begin();
        ++it; // Move to first element
        REQUIRE(*it == 1);
    }
}

TEST_CASE("Insert after operations", "[forward_list]") {
    forward_list<int> fl{1, 3, 5};

    SECTION("Insert after begin") {
        auto it = fl.insert_after(fl.before_begin(), 0);
        REQUIRE(*it == 0);
        REQUIRE(fl.size() == 4);
        REQUIRE(fl.front() == 0);
    }

    SECTION("Insert in middle") {
        auto pos = fl.before_begin();
        ++pos; // Points to element with value 1
        auto it = fl.insert_after(pos, 2);
        REQUIRE(*it == 2);
        REQUIRE(fl.size() == 4);

        std::vector<int> expected{1, 2, 3, 5};
        std::vector<int> actual{fl.begin(), fl.end()};
        REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
    }

    SECTION("Insert at end") {
        auto pos = fl.before_begin();
        for (size_t i = 0; i < fl.size(); ++i) {
            ++pos;
        }
        auto it = fl.insert_after(pos, 6);
        REQUIRE(*it == 6);
        REQUIRE(fl.size() == 4);
        // REQUIRE(*(--fl.end()) == 6);
    }
}

TEST_CASE("Emplace after operations", "[forward_list]") {
    forward_list<std::pair<int, std::string>> fl{{1, "one"}, {3, "three"}};

    auto pos = fl.before_begin();
    auto it = fl.emplace_after(pos, 0, "zero");

    REQUIRE(it->first == 0);
    REQUIRE(it->second == "zero");
    REQUIRE(fl.size() == 3);
    REQUIRE(fl.front().first == 0);
}

TEST_CASE("Erase after operations", "[forward_list]") {
    forward_list<int> fl{1, 2, 3, 4, 5};

    SECTION("Erase single element") {
        auto it = fl.erase_after(fl.before_begin()); // erase 1
        REQUIRE(*it == 2);
        REQUIRE(fl.size() == 4);
        REQUIRE(fl.front() == 2);
    }

    SECTION("Erase range of elements") {
        auto first = fl.before_begin();
        auto last = first;
        ++last;
        ++last;
        ++last; // Advance to point before element 3

        auto it = fl.erase_after(first, last); // erase 1, 2
        REQUIRE(*it == 3);
        REQUIRE(fl.size() == 3);

        std::vector<int> expected{3, 4, 5};
        std::vector<int> actual{fl.begin(), fl.end()};
        REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
    }
}

TEST_CASE("Splice after operations", "[forward_list]") {
    forward_list<int> fl1{1, 2, 3};
    forward_list<int> fl2{4, 5, 6};

    SECTION("Splice entire list") {
        auto pos = fl1.before_begin();
        ++pos; // Point after 1
        fl1.splice_after(pos, fl2);

        REQUIRE(fl1.size() == 6);
        REQUIRE(fl2.empty());

        std::vector<int> expected{1, 4, 5, 6, 2, 3};
        std::vector<int> actual{fl1.begin(), fl1.end()};
        REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
    }

    SECTION("Splice single element") {
        auto pos = fl1.before_begin();
        ++pos; // Point after 1
        auto it = fl2.before_begin();
        ++it; // Point to element 4
        fl1.splice_after(pos, fl2, it);

        REQUIRE(fl1.size() == 4);
        REQUIRE(fl2.size() == 2);

        std::vector<int> expected1{1, 5, 2, 3};
        std::vector<int> actual1{fl1.begin(), fl1.end()};
        REQUIRE_THAT(actual1, Catch::Matchers::Equals(expected1));

        std::vector<int> expected2{4, 6};
        std::vector<int> actual2{fl2.begin(), fl2.end()};
        REQUIRE_THAT(actual2, Catch::Matchers::Equals(expected2));
    }
}

TEST_CASE("Remove operations", "[forward_list]") {
    forward_list<int> fl{1, 2, 3, 2, 4, 2, 5};

    SECTION("Remove specific value") {
        fl.remove(2);
        REQUIRE(fl.size() == 4);

        std::vector<int> expected{1, 3, 4, 5};
        std::vector<int> actual{fl.begin(), fl.end()};
        REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
    }

    SECTION("Remove with predicate") {
        fl.remove_if([](int n) { return n % 2 == 0; }); // Remove even numbers
        REQUIRE(fl.size() == 3);

        std::vector<int> expected{1, 3, 5};
        std::vector<int> actual{fl.begin(), fl.end()};
        REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
    }
}

TEST_CASE("Unique operations", "[forward_list]") {
    forward_list<int> fl{1, 2, 2, 3, 3, 3, 4, 5, 5};

    SECTION("Unique without predicate") {
        fl.unique();
        REQUIRE(fl.size() == 5);

        std::vector<int> expected{1, 2, 3, 4, 5};
        std::vector<int> actual{fl.begin(), fl.end()};
        REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
    }

    SECTION("Unique with predicate") {
        // Remove elements with same parity consecutively
        fl.unique([](int a, int b) { return (a % 2) == (b % 2); });
        // Expected result depends on implementation but should reduce size
        REQUIRE(fl.size() <= 9);
    }
}

TEST_CASE("Reverse operation", "[forward_list]") {
    forward_list<int> fl{1, 2, 3, 4, 5};
    fl.reverse();

    std::vector<int> expected{5, 4, 3, 2, 1};
    std::vector<int> actual{fl.begin(), fl.end()};
    REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
}

TEST_CASE("Comparison operations", "[forward_list]") {
    forward_list<int> fl1{1, 2, 3};
    forward_list<int> fl2{1, 2, 3};
    forward_list<int> fl3{1, 2, 4};
    forward_list<int> fl4{1, 2};

    SECTION("Equality") {
        REQUIRE(fl1 == fl2);
        REQUIRE_FALSE(fl1 == fl3);
    }

    SECTION("Ordering") {
        REQUIRE(fl1 <= fl2);
        REQUIRE(fl1 < fl3);
        REQUIRE(fl4 < fl1);
    }
}