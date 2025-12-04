# 不需要构建库，只需要确保头文件正确包含
# 这个目录主要包含模板实现文件（.tpp）
# CMake会自动处理头文件和模板文件的包含

# 添加一个自定义目标来确保模板文件被正确处理
add_custom_target(template_sources SOURCES
    forward_list.tpp
    list.tpp
)

# 如果有非模板的源文件需要编译，可以在这里添加
# add_library(datastructures STATIC vector.cpp)