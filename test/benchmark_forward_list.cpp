#include <benchmark/benchmark.h>
#include <forward_list>
#include <vector>
#include <random>
#include <algorithm>
#include "forward_list.h"

// 测试数据大小
constexpr int test_size = 1000;

// 生成随机数的辅助函数
std::vector<int> generate_random_data(int size) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < size; ++i) {
        data[i] = dis(gen);
    }
    return data;
}

// 测试 mys::forward_list 的插入性能 (头部插入)
static void BM_MyForwardList_PushFront(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        mys::forward_list<int> list;
        for (int i = 0; i < test_size; ++i) {
            list.push_front(test_data[i]);
        }
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_MyForwardList_PushFront);

// 测试 std::forward_list 的插入性能 (头部插入)
static void BM_StdForwardList_PushFront(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        std::forward_list<int> list;
        for (int i = 0; i < test_size; ++i) {
            list.push_front(test_data[i]);
        }
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_StdForwardList_PushFront);

// 测试 mys::forward_list 的遍历性能
static void BM_MyForwardList_Iteration(benchmark::State &state) {
    mys::forward_list<int> list;
    auto test_data = generate_random_data(test_size);
    for (const auto &val : test_data) {
        list.push_front(val);
    }

    for (auto _ : state) {
        int sum = 0;
        for (const auto &val : list) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_MyForwardList_Iteration);

// 测试 std::forward_list 的遍历性能
static void BM_StdForwardList_Iteration(benchmark::State &state) {
    std::forward_list<int> list;
    auto test_data = generate_random_data(test_size);
    for (const auto &val : test_data) {
        list.push_front(val);
    }

    for (auto _ : state) {
        int sum = 0;
        for (const auto &val : list) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_StdForwardList_Iteration);

// 测试 mys::forward_list 的查找性能
static void BM_MyForwardList_Find(benchmark::State &state) {
    mys::forward_list<int> list;
    auto test_data = generate_random_data(test_size);
    for (const auto &val : test_data) {
        list.push_front(val);
    }

    for (auto _ : state) {
        // 查找中间值
        auto it = std::find(list.begin(), list.end(), test_data[test_size / 2]);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(BM_MyForwardList_Find);

// 测试 std::forward_list 的查找性能
static void BM_StdForwardList_Find(benchmark::State &state) {
    std::forward_list<int> list;
    auto test_data = generate_random_data(test_size);
    for (const auto &val : test_data) {
        list.push_front(val);
    }

    for (auto _ : state) {
        // 查找中间值
        auto it = std::find(list.begin(), list.end(), test_data[test_size / 2]);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(BM_StdForwardList_Find);

// 测试 mys::forward_list 的删除性能
static void BM_MyForwardList_Remove(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        mys::forward_list<int> list;
        for (const auto &val : test_data) {
            list.push_front(val);
        }
        state.ResumeTiming();

        list.remove(test_data[test_size / 2]);
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_MyForwardList_Remove);

// 测试 std::forward_list 的删除性能
static void BM_StdForwardList_Remove(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        std::forward_list<int> list;
        for (const auto &val : test_data) {
            list.push_front(val);
        }
        state.ResumeTiming();

        list.remove(test_data[test_size / 2]);
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_StdForwardList_Remove);

// 测试 mys::forward_list 的反转性能
static void BM_MyForwardList_Reverse(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        mys::forward_list<int> list;
        for (const auto &val : test_data) {
            list.push_front(val);
        }
        state.ResumeTiming();

        list.reverse();
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_MyForwardList_Reverse);

// 测试 std::forward_list 的反转性能
static void BM_StdForwardList_Reverse(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        std::forward_list<int> list;
        for (const auto &val : test_data) {
            list.push_front(val);
        }
        state.ResumeTiming();

        list.reverse();
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_StdForwardList_Reverse);

// 测试 mys::forward_list 的拼接性能
static void BM_MyForwardList_Splice(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        mys::forward_list<int> list1, list2;
        for (int i = 0; i < test_size / 2; ++i) {
            list1.push_front(test_data[i]);
            list2.push_front(test_data[test_size / 2 + i]);
        }
        state.ResumeTiming();

        list1.splice_after(list1.before_begin(), list2);
        benchmark::DoNotOptimize(list1);
    }
}
BENCHMARK(BM_MyForwardList_Splice);

// 测试 std::forward_list 的拼接性能
static void BM_StdForwardList_Splice(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        std::forward_list<int> list1, list2;
        for (int i = 0; i < test_size / 2; ++i) {
            list1.push_front(test_data[i]);
            list2.push_front(test_data[test_size / 2 + i]);
        }
        state.ResumeTiming();

        list1.splice_after(list1.before_begin(), list2);
        benchmark::DoNotOptimize(list1);
    }
}
BENCHMARK(BM_StdForwardList_Splice);

// 测试 mys::forward_list 的去重性能
static void BM_MyForwardList_Unique(benchmark::State &state) {
    for (auto _ : state) {
        state.PauseTiming();
        mys::forward_list<int> list;
        // 创建有重复元素的列表
        for (int i = 0; i < test_size; ++i) {
            list.push_front(i / 2); // 每个数字出现两次
        }
        state.ResumeTiming();

        list.unique();
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_MyForwardList_Unique);

// 测试 std::forward_list 的去重性能
static void BM_StdForwardList_Unique(benchmark::State &state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::forward_list<int> list;
        // 创建有重复元素的列表
        for (int i = 0; i < test_size; ++i) {
            list.push_front(i / 2); // 每个数字出现两次
        }
        state.ResumeTiming();

        list.unique();
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_StdForwardList_Unique);

BENCHMARK_MAIN();