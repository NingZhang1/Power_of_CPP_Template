cmake_minimum_required(VERSION 3.10)
project(BDF_Programmer_Training VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(OpenMP)

find_package(MKL REQUIRED)

get_target_property(MKL_INCLUDE_DIRS MKL::MKL INTERFACE_INCLUDE_DIRECTORIES)

link_directories(${MKLROOT}/lib/intel64)

include_directories(${MKLROOT}/include)

add_executable(poly_function_overloading 01-poly-function_overloading.cpp)
add_executable(poly_operator_overloading 01-poly-operator_overloading.cpp)
add_executable(poly_specialization 02-poly-specialization.cpp)
add_executable(poly_ABC 03-poly-ABC_virtual_functions.cpp)
add_executable(type_traits 04-type_traits.cpp)
add_executable(concept 05-concept.cpp)
add_executable(expression_template 06-expression_template.cpp)

# 定义目标列表
set(TARGETS
    poly_function_overloading
    poly_operator_overloading
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