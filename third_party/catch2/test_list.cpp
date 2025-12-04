#include "list.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <string>
#include <vector>

using namespace mys;

TEST_CASE("Default construction", "[list]") {
    list<int> l;
    REQUIRE(l.empty());
    REQUIRE(l.size() == 0);
}

TEST_CASE("Initializer list construction", "[list]") {
    list<int> l{1, 2, 3, 4, 5};
    REQUIRE_FALSE(l.empty());
    REQUIRE(l.size() == 5);
    REQUIRE(l.front() == 1);
    REQUIRE(l.back() == 5);
}

TEST_CASE("Copy construction", "[list]") {
    list<int> original{1, 2, 3};
    list<int> copy(original);

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.front() == 1);
    REQUIRE(copy.back() == 3);

    // 修改原列表不应影响复制的列表
    original.push_back(4);
    REQUIRE(original.size() == 4);
    REQUIRE(copy.size() == 3);
}

TEST_CASE("Move construction", "[list]") {
    list<int> original{1, 2, 3};
    list<int> moved(std::move(original));

    REQUIRE(moved.size() == 3);
    REQUIRE(moved.front() == 1);
    REQUIRE(moved.back() == 3);
    // 原列表状态未定义，但应该可以安全析构
}

TEST_CASE("Assignment operators", "[list]") {
    list<int> l1{1, 2, 3};
    list<int> l2;

    SECTION("Copy assignment") {
        l2 = l1;
        REQUIRE(l2.size() == 3);
        REQUIRE(l2.front() == 1);
        REQUIRE(l2.back() == 3);
    }

    SECTION("Move assignment") {
        list<int> l3{4, 5};
        l3 = std::move(l1);
        REQUIRE(l3.size() == 3);
        REQUIRE(l3.front() == 1);
        REQUIRE(l3.back() == 3);
    }
}

TEST_CASE("Element access", "[list]") {
    list<std::string> l{"hello", "world"};

    SECTION("Non-const access") {
        REQUIRE(l.front() == "hello");
        REQUIRE(l.back() == "world");

        l.front() = "hi";
        REQUIRE(l.front() == "hi");
    }

    SECTION("Const access") {
        const auto &cl = l;
        REQUIRE(cl.front() == "hello");
        REQUIRE(cl.back() == "world");
    }
}

TEST_CASE("Push operations", "[list]") {
    list<int> l;

    SECTION("push_back") {
        l.push_back(1);
        REQUIRE(l.size() == 1);
        REQUIRE(l.back() == 1);

        l.push_back(2);
        REQUIRE(l.size() == 2);
        REQUIRE(l.back() == 2);
    }

    SECTION("push_front") {
        l.push_front(1);
        REQUIRE(l.size() == 1);
        REQUIRE(l.front() == 1);

        l.push_front(2);
        REQUIRE(l.size() == 2);
        REQUIRE(l.front() == 2);
    }

    SECTION("Mixed push operations") {
        l.push_back(1);
        l.push_front(0);
        l.push_back(2);

        REQUIRE(l.size() == 3);
        REQUIRE(l.front() == 0);
        REQUIRE(l.back() == 2);
    }
}

TEST_CASE("Emplace operations", "[list]") {
    list<std::pair<int, std::string>> l;

    l.emplace_back(1, "one");
    l.emplace_front(0, "zero");

    REQUIRE(l.size() == 2);
    REQUIRE(l.front().first == 0);
    REQUIRE(l.front().second == "zero");
    REQUIRE(l.back().first == 1);
    REQUIRE(l.back().second == "one");
}

TEST_CASE("Pop operations", "[list]") {
    list<int> l{1, 2, 3, 4, 5};

    SECTION("pop_back") {
        l.pop_back();
        REQUIRE(l.size() == 4);
        REQUIRE(l.back() == 4);

        l.pop_back();
        REQUIRE(l.size() == 3);
        REQUIRE(l.back() == 3);
    }

    SECTION("pop_front") {
        l.pop_front();
        REQUIRE(l.size() == 4);
        REQUIRE(l.front() == 2);

        l.pop_front();
        REQUIRE(l.size() == 3);
        REQUIRE(l.front() == 3);
    }
}

TEST_CASE("Clear operation", "[list]") {
    list<int> l{1, 2, 3};
    REQUIRE(l.size() == 3);

    l.clear();
    REQUIRE(l.empty());
    REQUIRE(l.size() == 0);
}

TEST_CASE("Swap operation", "[list]") {
    list<int> l1{1, 2, 3};
    list<int> l2{4, 5};

    l1.swap(l2);

    REQUIRE(l1.size() == 2);
    REQUIRE(l1.front() == 4);
    REQUIRE(l1.back() == 5);

    REQUIRE(l2.size() == 3);
    REQUIRE(l2.front() == 1);
    REQUIRE(l2.back() == 3);
}

TEST_CASE("Iterator operations", "[list]") {
    list<int> l{1, 2, 3, 4, 5};

    SECTION("Begin/End iterators") {
        auto it = l.begin();
        REQUIRE(*it == 1);

        auto end_it = l.end();
        REQUIRE(it != end_it);
    }

    SECTION("Iterator traversal forward") {
        std::vector<int> values;
        for (auto it = l.begin(); it != l.end(); ++it) {
            values.push_back(*it);
        }

        std::vector<int> expected{1, 2, 3, 4, 5};
        REQUIRE_THAT(values, Catch::Matchers::Equals(expected));
    }

    SECTION("Iterator traversal backward") {
        std::vector<int> values;
        for (auto it = l.end(); it != l.begin();) {
            --it;
            values.push_back(*it);
        }

        std::vector<int> expected{5, 4, 3, 2, 1};
        REQUIRE_THAT(values, Catch::Matchers::Equals(expected));
    }

    SECTION("Const iterators") {
        const list<int> &cl = l;
        std::vector<int> values;
        for (auto it = cl.cbegin(); it != cl.cend(); ++it) {
            values.push_back(*it);
        }

        std::vector<int> expected{1, 2, 3, 4, 5};
        REQUIRE_THAT(values, Catch::Matchers::Equals(expected));
    }
}

TEST_CASE("Insert operations", "[list]") {
    list<int> l{1, 3, 5};

    SECTION("Insert at beginning") {
        auto it = l.insert(l.begin(), 0);
        REQUIRE(*it == 0);
        REQUIRE(l.size() == 4);
        REQUIRE(l.front() == 0);
    }

    SECTION("Insert at middle") {
        auto it = l.insert(++l.begin(), 2);
        REQUIRE(*it == 2);
        REQUIRE(l.size() == 4);

        std::vector<int> expected{1, 2, 3, 5};
        std::vector<int> actual{l.begin(), l.end()};
        REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
    }

    SECTION("Insert at end") {
        auto it = l.insert(l.end(), 6);
        REQUIRE(*it == 6);
        REQUIRE(l.size() == 4);
        REQUIRE(l.back() == 6);
    }
}

TEST_CASE("Erase operations", "[list]") {
    list<int> l{1, 2, 3, 4, 5};

    SECTION("Erase single element") {
        auto it = l.erase(++l.begin()); // erase 2
        REQUIRE(*it == 3);
        REQUIRE(l.size() == 4);

        std::vector<int> expected{1, 3, 4, 5};
        std::vector<int> actual{l.begin(), l.end()};
        REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
    }

    SECTION("Erase range of elements") {
        auto it = l.erase(++l.begin(), --l.end()); // erase 2,3,4
        REQUIRE(*it == 5);
        REQUIRE(l.size() == 2);

        std::vector<int> expected{1, 5};
        std::vector<int> actual{l.begin(), l.end()};
        REQUIRE_THAT(actual, Catch::Matchers::Equals(expected));
    }
}

TEST_CASE("Reverse iterators", "[list]") {
    list<int> l{1, 2, 3, 4, 5};

    std::vector<int> reversed{l.rbegin(), l.rend()};
    std::vector<int> expected{5, 4, 3, 2, 1};

    REQUIRE_THAT(reversed, Catch::Matchers::Equals(expected));
}

TEST_CASE("Comparison operations", "[list]") {
    list<int> l1{1, 2, 3};
    list<int> l2{1, 2, 3};
    list<int> l3{1, 2, 4};
    list<int> l4{1, 2};

    SECTION("Equality") {
        REQUIRE(l1 == l2);
        REQUIRE_FALSE(l1 == l3);
    }

    SECTION("Ordering") {
        REQUIRE(l1 <= l2);
        REQUIRE(l1 < l3);
        REQUIRE(l4 < l1);
    }
}