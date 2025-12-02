cmake_minimum_required(VERSION 3.10)
project(MyStructure)

# 设置C++标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(include)
include_directories(src)  

# 查找并链接Google Benchmark库（如果后续要用的话）
find_package(benchmark REQUIRED)

# 创建可执行文件用于单链表测试
add_executable(test_forward_list
    test/test_forward_list.cpp
    # src/forward_list.tpp
)
add_executable(benchmark_forward_list
    test/benchmark_forward_list.cpp
    # src/forward_list.tpp
)

# 创建可执行文件用于双向链表测试
add_executable(test_list
    test/test_list.cpp
    # src/list.tpp  
)
add_executable(benchmark_list
    test/benchmark_list.cpp
    # src/list.tpp  
)

# 链接Google Benchmark库
target_link_libraries(benchmark_forward_list benchmark::benchmark)
target_link_libraries(benchmark_list benchmark::benchmark)

# 启用测试功能
enable_testing()

# 添加测试
add_test(NAME TestForwardList COMMAND test_forward_list)
add_test(NAME BenchmarkTestForwardList COMMAND benchmark_forward_list)
add_test(NAME TestList COMMAND test_list)
add_test(NAME BenchmarkTestList COMMAND benchmark_list)
