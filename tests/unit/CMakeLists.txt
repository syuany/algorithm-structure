# 单元测试构建配置

# 设置测试可执行文件输出目录
set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/tests)

# 查找测试源文件
file(GLOB TEST_SOURCES "*.cpp")

foreach(test_source ${TEST_SOURCES})
    # 从文件名提取测试名称（去掉扩展名和test_前缀）
    get_filename_component(test_name ${test_source} NAME_WE)
    string(REPLACE "test_" "" test_base ${test_name})
    
    # 创建测试可执行文件
    add_executable(${test_name} ${test_source})
    
    # 设置输出目录
    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
    )
    
    # 链接必要的库
    # if(${test_name} STREQUAL "test_list")
    #     # 如果有list的特定依赖，可以在这里添加
    #     target_link_libraries(${test_name})
    # elseif(${test_name} STREQUAL "test_forward_list")
    #     # forward_list的特定依赖
    #     target_link_libraries(${test_name})
    # endif()
    
    # 添加测试到CTest
    add_test(NAME ${test_name}
             COMMAND ${TEST_OUTPUT_DIR}/${test_name}
             WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    
    # 设置测试属性
    set_tests_properties(${test_name} PROPERTIES
        TIMEOUT 30
        LABELS "unit"
    )
endforeach()