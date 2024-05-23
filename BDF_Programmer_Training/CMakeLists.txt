cmake_minimum_required(VERSION 3.10)
project(BDF_Programmer_Training VERSION 1.0)

# 查找 OpenMP
find_package(OpenMP)

# 添加 MKL 库的路径到链接路径
link_directories(${MKLROOT}/lib/intel64)

# 添加 MKL 库的头文件路径到包含路径
include_directories(${MKLROOT}/include)

# 获取当前目录下所有的.cpp文件
file(GLOB CPP_SOURCES "*.cpp")

# 为每个.cpp文件创建一个可执行文件
foreach(SOURCE ${CPP_SOURCES})
    # 获取文件名，不包含扩展名
    get_filename_component(EXECUTABLE_NAME ${SOURCE} NAME_WE)
    # 为每个源文件添加一个可执行目标
    add_executable(${EXECUTABLE_NAME} ${SOURCE})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "${EXECUTABLE_NAME}.exe")
    # 链接 MKL 库
    target_link_libraries(${EXECUTABLE_NAME} mkl_intel_lp64 mkl_sequential mkl_core pthread m dl OpenMP::OpenMP_CXX)
endforeach()
