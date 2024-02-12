
# 指定不参与单独编译成可执行文件的源文件
set(SPECIAL_SOURCES source1.cpp source2.cpp)

# 创建一个静态库
add_library(SpecialLib STATIC ${SPECIAL_SOURCES})

# 为Main.cpp创建一个可执行文件，并链接之前创建的静态库
add_executable(SpecialExecutable 02-Template_Instantiation.cpp)
target_link_libraries(SpecialExecutable PRIVATE SpecialLib)
set_target_properties(SpecialExecutable PROPERTIES OUTPUT_NAME "02-Template_Instantiation.exe")

# 获取当前目录下所有的.cpp文件
file(GLOB ALL_CPP_SOURCES "*.cpp")

# 为除了 source1.cpp, source1.cpp, 02-Template_Instantiation.cpp之外的每个.cpp文件创建一个可执行文件
foreach(SOURCE ${ALL_CPP_SOURCES})
    # 获取文件名，不包含扩展名
    get_filename_component(FILE_NAME ${SOURCE} NAME)
    
    # 跳过特殊处理的源文件
    if(NOT FILE_NAME IN_LIST SPECIAL_SOURCES AND NOT FILE_NAME STREQUAL "02-Template_Instantiation.cpp")
        # 获取可执行文件名（不包含扩展名）
        get_filename_component(EXECUTABLE_NAME ${SOURCE} NAME_WE)
        # 为每个源文件添加一个可执行目标
        add_executable(${EXECUTABLE_NAME} ${SOURCE})
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "${EXECUTABLE_NAME}.exe")
    endif()
endforeach()
