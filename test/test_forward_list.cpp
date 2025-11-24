#include <benchmark/benchmark.h>
#include <forward_list>
#include <vector>
#include <random>
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
static void BM_forward_list_PushFront(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        mys::forward_list<int> list;
        for (int i = 0; i < test_size; ++i) {
            list.push_front(test_data[i]);
        }
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_forward_list_PushFront);

// 测试 std::forward_list 的插入性能 (头部插入)
static void BM_ForwardList_PushFront(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        std::forward_list<int> list;
        for (int i = 0; i < test_size; ++i) {
            list.push_front(test_data[i]);
        }
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_ForwardList_PushFront);

// 测试 mys::forward_list 的遍历性能
static void BM_forward_list_Iteration(benchmark::State &state) {
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
BENCHMARK(BM_forward_list_Iteration);

// 测试 std::forward_list 的遍历性能
static void BM_ForwardList_Iteration(benchmark::State &state) {
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
BENCHMARK(BM_ForwardList_Iteration);

// 测试 mys::forward_list 的查找性能
static void BM_forward_list_Find(benchmark::State &state) {
    mys::forward_list<int> list;
    auto test_data = generate_random_data(test_size);
    for (const auto &val : test_data) {
        list.push_front(val);
    }

    for (auto _ : state) {
        // 查找中间值
        auto it = list.find(test_data[test_size / 2]);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(BM_forward_list_Find);

// 测试 std::forward_list 的查找性能
static void BM_ForwardList_Find(benchmark::State &state) {
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
BENCHMARK(BM_ForwardList_Find);

// 测试 mys::forward_list 的删除性能
static void BM_forward_list_Remove(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        mys::forward_list<int> list;
        for (const auto &val : test_data) {
            list.push_front(val);
        }
        state.ResumeTiming();

        list.remove_value(test_data[test_size / 2]);
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_forward_list_Remove);

// 测试 std::forward_list 的删除性能
static void BM_ForwardList_Remove(benchmark::State &state) {
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
BENCHMARK(BM_ForwardList_Remove);

// 测试 mys::forward_list 的反转性能
static void BM_forward_list_Reverse(benchmark::State &state) {
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
BENCHMARK(BM_forward_list_Reverse);

// 测试 std::forward_list 的反转性能
static void BM_ForwardList_Reverse(benchmark::State &state) {
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
BENCHMARK(BM_ForwardList_Reverse);

BENCHMARK_MAIN();