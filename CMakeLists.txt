cmake_minimum_required(VERSION 3.13)
project(plumber)

# 设置 IrrKlang 版本和下载链接
set(IRRKLANG_VERSION 1.6.0)
include(FetchContent)
FetchContent_Declare(
    irrKlang
    URL https://www.ambiera.at/downloads/irrKlang-64bit-${IRRKLANG_VERSION}.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(irrKlang)

# 假定 IrrKlang 解压到了预设的 irrKlang_SOURCE_DIR
set(IRRKLANG_ROOT "build/_deps/irrklang-src")

# 创建可执行文件
add_executable(plumber Algorithm.h Direction.h Game.h Maze.h Pipe.h Source.cpp)

target_include_directories(plumber PUBLIC ".")
target_include_directories(plumber PUBLIC ${IRRKLANG_ROOT}/include)
# 链接 IrrKlang 库
target_link_directories(plumber PRIVATE ${IRRKLANG_ROOT}/lib/Winx64-visualStudio)
target_link_libraries(plumber PRIVATE irrKlang)

# 复制音效文件夹到构建目录，以便运行时可以找到音效资源
file(COPY ${CMAKE_SOURCE_DIR}/soundEffect DESTINATION ${CMAKE_BINARY_DIR}/soundEffect)

add_custom_command(TARGET plumber POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${IRRKLANG_ROOT}/bin/winx64-visualStudio/irrKlang.dll ${CMAKE_BINARY_DIR}/Debug/irrKlang.dll)

# 对于每种构建配置（Debug, Release等），确保运行时资源也被复制过去
foreach(CONFIGURATION_TYPE IN LISTS CMAKE_CONFIGURATION_TYPES)
    add_custom_command(
        TARGET plumber POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/soundEffect
                ${CMAKE_BINARY_DIR}/${CONFIGURATION_TYPE}/soundEffect
    )
endforeach()
