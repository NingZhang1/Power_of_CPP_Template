cmake_minimum_required(VERSION 3.10)
project(CPPTemplateProgramming VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置优化级别为-O3
add_compile_options(-O3)
    
# 包含子目录
add_subdirectory(Template_MetaProgramming)
add_subdirectory(STL)
add_subdirectory(MetaProgramming)
add_subdirectory(BDF_Programmer_Training)