cmake_minimum_required(VERSION 3.10)
project(MyStructure)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置C++标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(include)
include_directories(src)  

# 查找并链接Google Benchmark库
find_package(benchmark REQUIRED)

# 创建可执行文件用于单链表测试
add_executable(test_forward_list
    test/test_forward_list.cpp
    src/forward_list.tpp
)

# 链接Google Benchmark库
target_link_libraries(test_forward_list benchmark::benchmark)

# 启用测试功能
enable_testing()

# 添加测试
add_test(NAME ListTest COMMAND test_forward_list)
# add_test(NAME BenchmarkTest COMMAND benchmark_test)