// benchmark_list.cpp
#include "list.h"
#include <benchmark/benchmark.h>
#include <list>
#include <vector>
#include <random>

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

// 测试 mys::list 的 push_back 性能
static void BM_List_PushBack(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        mys::list<int> l;
        for (int i = 0; i < test_size; ++i) {
            l.push_back(test_data[i]);
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_List_PushBack);

// 测试 std::list 的 push_back 性能作为对比
static void BM_StdList_PushBack(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        std::list<int> l;
        for (int i = 0; i < test_size; ++i) {
            l.push_back(test_data[i]);
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_StdList_PushBack);

// 测试 mys::list 的 push_front 性能
static void BM_List_PushFront(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        mys::list<int> l;
        for (int i = 0; i < test_size; ++i) {
            l.push_front(test_data[i]);
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_List_PushFront);

// 测试 std::list 的 push_front 性能作为对比
static void BM_StdList_PushFront(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        std::list<int> l;
        for (int i = 0; i < test_size; ++i) {
            l.push_front(test_data[i]);
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_StdList_PushFront);

// 测试 mys::list 的遍历性能
static void BM_List_Iteration(benchmark::State &state) {
    mys::list<int> l;
    auto test_data = generate_random_data(test_size);
    for (const auto &val : test_data) {
        l.push_back(val);
    }

    for (auto _ : state) {
        int sum = 0;
        for (const auto &val : l) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_List_Iteration);

// 测试 std::list 的遍历性能作为对比
static void BM_StdList_Iteration(benchmark::State &state) {
    std::list<int> l;
    auto test_data = generate_random_data(test_size);
    for (const auto &val : test_data) {
        l.push_back(val);
    }

    for (auto _ : state) {
        int sum = 0;
        for (const auto &val : l) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_StdList_Iteration);

// 测试 mys::list 的插入性能 (在开始位置插入)
static void BM_List_Insert(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        mys::list<int> l;
        state.ResumeTiming();

        auto it = l.begin();
        for (int i = 0; i < test_size; ++i) {
            it = l.insert(it, test_data[i]);
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_List_Insert);

// 测试 std::list 的插入性能 (在开始位置插入)作为对比
static void BM_StdList_Insert(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        std::list<int> l;
        state.ResumeTiming();

        auto it = l.begin();
        for (int i = 0; i < test_size; ++i) {
            it = l.insert(it, test_data[i]);
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_StdList_Insert);

// 测试 mys::list 的 erase 性能
static void BM_List_Erase(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        mys::list<int> l;
        for (const auto &val : test_data) {
            l.push_back(val);
        }
        state.ResumeTiming();

        while (!l.empty()) {
            l.erase(l.begin());
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_List_Erase);

// 测试 std::list 的 erase 性能作为对比
static void BM_StdList_Erase(benchmark::State &state) {
    auto test_data = generate_random_data(test_size);
    for (auto _ : state) {
        state.PauseTiming();
        std::list<int> l;
        for (const auto &val : test_data) {
            l.push_back(val);
        }
        state.ResumeTiming();

        while (!l.empty()) {
            l.erase(l.begin());
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_StdList_Erase);

BENCHMARK_MAIN();