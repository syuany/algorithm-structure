cmake_minimum_required(VERSION 3.15)
project(DataStructures LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置构建类型，如果未指定则默认为Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 启用测试 - 必须在所有add_test之前
enable_testing()

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(include/structure)
include_directories(src)

# 添加子目录
add_subdirectory(src)
# add_subdirectory(tests)
add_subdirectory(tests/unit)
add_subdirectory(third_party/google-benchmark)
add_subdirectory(third_party/google-test)
add_subdirectory(third_party/catch2)

# 设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra -pedantic -fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
endif()

# 查找性能测试数据生成脚本
find_program(PYTHON_EXECUTABLE python3 python)
if(PYTHON_EXECUTABLE)
    add_custom_target(
        generate_test_data
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/gen_test_data.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "生成测试数据..."
    )
endif()

# 安装配置（可选）
install(DIRECTORY include/structure DESTINATION include)
install(FILES README.md DESTINATION .)