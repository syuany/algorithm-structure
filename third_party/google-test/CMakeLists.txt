# Google Test配置

# 下载和构建Google Test
include(FetchContent)

# FetchContent_Declare(
#     googletest
#     GIT_REPOSITORY https://github.com/google/googletest.git
#     GIT_TAG release-1.12.1  # 使用稳定版本
# )

find_package(GTest REQUIRED)

# 设置为离线模式，防止每次构建都下载
# set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
# FetchContent_MakeAvailable(googletest)

# 创建使用gtest的测试
add_executable(test_list_gtest test_list.cpp)
add_executable(test_forward_list_gtest test_forward_list.cpp)

# 链接gtest库
target_link_libraries(test_list_gtest GTest::gtest GTest::gtest_main)
target_link_libraries(test_forward_list_gtest GTest::gtest GTest::gtest_main)

# 包含目录
target_include_directories(test_list_gtest PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(test_forward_list_gtest PRIVATE ${PROJECT_SOURCE_DIR}/include)

# 添加测试
add_test(NAME test_list_gtest COMMAND test_list_gtest)
add_test(NAME test_forward_list_gtest COMMAND test_forward_list_gtest)