#include <benchmark/benchmark.h>
#include <array>

constexpr int len = 6;

// constexpr function具有inline属性，你应该把它放在头文件中
constexpr auto my_pow(const int i) {
    return i * i;
}

// 使用operator[]读取元素，依次存入1-6的平方
static void bench_array_operator(benchmark::State &state) {
    std::array<int, len> arr;
    constexpr int i = 1;
    for (auto _ : state) {
        arr[0] = my_pow(i);
        arr[1] = my_pow(i + 1);
        arr[2] = my_pow(i + 2);
        arr[3] = my_pow(i + 3);
        arr[4] = my_pow(i + 4);
        arr[5] = my_pow(i + 5);
    }
}
BENCHMARK(bench_array_operator);

// 使用at()读取元素，依次存入1-6的平方
static void bench_array_at(benchmark::State &state) {
    std::array<int, len> arr;
    constexpr int i = 1;
    for (auto _ : state) {
        arr.at(0) = my_pow(i);
        arr.at(1) = my_pow(i + 1);
        arr.at(2) = my_pow(i + 2);
        arr.at(3) = my_pow(i + 3);
        arr.at(4) = my_pow(i + 4);
        arr.at(5) = my_pow(i + 5);
    }
}
BENCHMARK(bench_array_at);

// std::get<>(array)是一个constexpr function，它会返回容器内元素的引用，并在编译期检查数组的索引是否正确
static void bench_array_get(benchmark::State &state) {
    std::array<int, len> arr;
    constexpr int i = 1;
    for (auto _ : state) {
        std::get<0>(arr) = my_pow(i);
        std::get<1>(arr) = my_pow(i + 1);
        std::get<2>(arr) = my_pow(i + 2);
        std::get<3>(arr) = my_pow(i + 3);
        std::get<4>(arr) = my_pow(i + 4);
        std::get<5>(arr) = my_pow(i + 5);
    }
}
BENCHMARK(bench_array_get);

// 我们可以看到每一个benchmark测试用例都是一个类型为std::function<void(benchmark::State &)> 的函数，其中benchmark::State &负责测试的运行及额外参数的传递。

// 随后我们使用for(auto _ : state) {}
// 来运行需要测试的内容，state会选择合适的次数来运行循环，时间的计算从循环内的语句开始
// 所以我们可以选择像例子中一样在for循环之外初始化测试环境，然后在循环体内编写需要测试的代码。

// 测试用例编写完成后我们需要使用BENCHMARK(<function_name>);
// 将我们的测试用例注册进benchmark，这样程序运行时才会执行我们的测试。

// 最后是用BENCHMARK_MAIN();
// 替代直接编写的main函数，它会处理命令行参数并运行所有注册过的测试用例生成测试结果。

// 示例中大量使用了constexpt，这是为了能在编译期计算出需要的数值避免对测试产生太多噪音。

BENCHMARK_MAIN();
