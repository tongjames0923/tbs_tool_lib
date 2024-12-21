CMAKE_MINIMUM_REQUIRED(VERSION 3.27.8)
PROJECT(tbs_tool_lib VERSION 0.1.0 LANGUAGES C CXX)

message(STATUS "tbs tool lib build")
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/runtime)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/runtime)
SET(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/impls)
SET(PROJECT_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(INLS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/include/inls)

OPTION(CMAKE_LIB_BUILD_STATIC "库文件是否为静态，否为动态库" ON)


IF (WIN32)
    MESSAGE(STATUS "WINDOWS PLATFORM")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_INSTALL_PREFIX "C:/msys64/clang64")
ELSEIF (APPLE)
    MESSAGE(STATUS "APPLE PLATFORM")
    set(CMAKE_INSTALL_PREFIX "/opt/homebrew/")

    SET(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX12.1.sdk)
ENDIF ()
SET(HEADER_PATH ${CMAKE_INSTALL_PREFIX}/include)
SET(LIB_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(BUILD_TESTER ON CACHE BOOL "构建测试器")

# 添加子目录
add_subdirectory(base)
add_subdirectory(log)
add_subdirectory(concurrency)
if (${BUILD_TESTER})
    add_subdirectory(tester)
endif ()

# 搜索源文件和头文件
MESSAGE(STATUS "searching for sources...")
FILE(GLOB_RECURSE SOURCES ${SOURCE_PATH}/**.cpp ${SOURCE_PATH}/**.c)
FOREACH (ITEM IN LISTS SOURCES)
    MESSAGE(STATUS "Source file: ${ITEM}")
ENDFOREACH ()

MESSAGE(STATUS "searching for headers...")
FILE(GLOB_RECURSE HEADERS ${PROJECT_HEAD}/**.h ${PROJECT_HEAD}/**.hpp)
FOREACH (ITEM IN LISTS HEADERS)
    MESSAGE(STATUS "header file: ${ITEM}")
ENDFOREACH ()

MESSAGE(STATUS "searching for headers inls...")
FILE(GLOB_RECURSE HEADER_INLS ${INLS_SOURCE}/**.cuh)
FOREACH (ITEM IN LISTS HEADER_INLS)
    MESSAGE(STATUS "head inls file: ${ITEM}")
ENDFOREACH ()

# 根据选项构建静态或动态库
IF (CMAKE_LIB_BUILD_STATIC)
    MESSAGE(STATUS "static lib build")
    ADD_LIBRARY(tbs_tool_lib STATIC ${HEADERS} ${HEADER_INLS} ${SOURCES})
ELSE ()
    MESSAGE(STATUS "dynamic lib build")
    ADD_LIBRARY(tbs_tool_lib SHARED ${HEADERS} ${HEADER_INLS} ${SOURCES})
ENDIF ()

# 设置包含目录
target_include_directories(tbs_tool_lib PRIVATE ${HEADER_PATH} ${PROJECT_HEAD} ${INLS_SOURCE})

## 添加依赖关系
#add_dependencies(tbs_tool_lib tbs_tool_base tbs_log tbs_tool_concurrency)

# 安装库文件
install(TARGETS tbs_tool_lib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# 安装头文件
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
