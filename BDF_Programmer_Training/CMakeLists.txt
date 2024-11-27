cmake_minimum_required(VERSION 3.10)
project(BDF_Programmer_Training VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2") # 全局 O2 优化
set(CMAKE_VERBOSE_MAKEFILE ON)

# 查找 OpenMP
find_package(OpenMP)

# 查找 MKL 
find_package(MKL REQUIRED)

# 添加 MKL 库的路径到链接路径
get_target_property(MKL_INCLUDE_DIRS MKL::MKL INTERFACE_INCLUDE_DIRECTORIES)

link_directories(${MKLROOT}/lib/intel64)

# 添加 MKL 库的头文件路径到包含路径
include_directories(${MKLROOT}/include)

add_executable(poly_overloading 01-poly-overloading.cpp)
add_executable(poly_specialization 02-poly-specialization.cpp)
add_executable(poly_ABC 03-poly-ABC-virtual-functions.cpp)
add_executable(concept 04-concept.cpp)
add_executable(expression_template 05-expression_template.cpp)

# 定义目标列表
set(TARGETS
    poly_overloading
    poly_specialization
    poly_ABC
    concept
    expression_template
)

# 遍历每个目标并设置包含目录
foreach(TARGET ${TARGETS})
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(${TARGET} PRIVATE MKL)
    target_link_libraries(${TARGET} PRIVATE MKL::MKL)
endforeach()