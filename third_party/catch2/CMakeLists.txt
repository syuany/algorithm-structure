# Catch2配置

# 使用FetchContent下载Catch2
include(FetchContent)

# FetchContent_Declare(
#     Catch2
#     GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#     GIT_TAG v2.13.9  # 稳定版本
# )

# FetchContent_MakeAvailable(Catch2)

# 或者使用单头文件版本
# file(DOWNLOAD 
#      https://github.com/catchorg/Catch2/releases/download/v2.13.9/catch.hpp
#      ${CMAKE_CURRENT_BINARY_DIR}/catch.hpp)

find_package(Catch2 REQUIRED)

# 创建Catch2测试
add_executable(test_list_catch test_list.cpp)
add_executable(test_forward_list_catch test_forward_list.cpp)

# 链接Catch2库（如果使用库版本）
# target_link_libraries(test_list_catch Catch2::Catch2)
target_link_libraries(test_list_catch PRIVATE Catch2::Catch2WithMain)
# target_link_libraries(test_forward_list_catch Catch2::Catch2)
target_link_libraries(test_forward_list_catch PRIVATE Catch2::Catch2WithMain)

# 或者使用单头文件版本时
# target_include_directories(test_list_catch PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# 包含项目头文件
target_include_directories(test_list_catch PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(test_forward_list_catch PRIVATE ${PROJECT_SOURCE_DIR}/include)

# 添加测试
add_test(NAME test_list_catch COMMAND test_list_catch)
add_test(NAME test_forward_list_catch COMMAND test_forward_list_catch)