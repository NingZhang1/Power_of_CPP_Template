# 获取当前目录下所有的.cpp文件
file(GLOB CPP_SOURCES "*.cpp")

# 为每个.cpp文件创建一个可执行文件
foreach(SOURCE ${CPP_SOURCES})
    # 获取文件名，不包含扩展名
    get_filename_component(EXECUTABLE_NAME ${SOURCE} NAME_WE)
    # 为每个源文件添加一个可执行目标
    add_executable(${EXECUTABLE_NAME} ${SOURCE})
    target_link_libraries(${EXECUTABLE_NAME} tbb)
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "${EXECUTABLE_NAME}.exe")
endforeach()
