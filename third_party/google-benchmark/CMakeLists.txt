# Google Benchmark配置

# 下载和构建Google Benchmark
include(FetchContent)

# FetchContent_Declare(
#     googlebenchmark
#     GIT_REPOSITORY https://github.com/google/benchmark.git
#     GIT_TAG v1.7.1  # 稳定版本
# )

# FetchContent_MakeAvailable(googlebenchmark)

find_package(benchmark REQUIRED)

# 创建性能测试可执行文件
add_executable(benchmark_list bench_list.cpp)
add_executable(benchmark_forward_list bench_forward_list.cpp)

# 链接benchmark库
target_link_libraries(benchmark_list benchmark::benchmark)
target_link_libraries(benchmark_forward_list benchmark::benchmark)

# 包含项目头文件
target_include_directories(benchmark_list PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(benchmark_forward_list PRIVATE ${PROJECT_SOURCE_DIR}/include)

# 设置性能测试属性
set_target_properties(benchmark_list benchmark_forward_list PROPERTIES
    EXCLUDE_FROM_DEFAULT_BUILD ON  # 默认不构建性能测试
)

# 添加自定义目标以便构建性能测试
add_custom_target(benchmarks
    DEPENDS benchmark_list benchmark_forward_list
    COMMENT "构建所有性能测试"
)