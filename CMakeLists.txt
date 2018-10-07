cmake_minimum_required(VERSION 2.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(Morisson)
#if(CMAKE_VERSION VERSION_GREATER 3)
    #cmake_policy(SET CMP0054 NEW)
#endif() 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules/})
message(${CMAKE_MODULE_PATH})

include_directories(${PROJECT_SOURCE_DIR}/ext/nanogui/include/)
include_directories(${PROJECT_SOURCE_DIR}/ext/nanogui/ext/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/ext/nanogui/ext/eigen)
include_directories(${PROJECT_SOURCE_DIR}/ext/nanogui/ext/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/ext/nanogui/ext/nanovg/src)
include_directories(${PROJECT_SOURCE_DIR}/ext/nanogui/build/)

include_directories(${PROJECT_SOURCE_DIR}/gui/)

include_directories(${PROJECT_SOURCE_DIR}/ext/)
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${GLM_INCLUDE_DIR})

message("source dir = ${PROJECT_SOURCE_DIR}")

file(GLOB MOR_SRC_FILES "./src/*.cpp")
file(GLOB MOR_HDR_FILES "./src/*.h")
file(GLOB GUI_SRC_FILES "./gui/*.cpp")
file(GLOB GUI_HDR_FILES "./gui/*.h")

ADD_EXECUTABLE(Morisson ${MOR_SRC_FILES} ${MOR_HDR_FILES} ${GUI_SRC_FILES} ${GUI_HDR_FILES} ${OPENGL_LIBRARIES})
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

IF(WIN32)
    set(NANOGUI_LIB "${PROJECT_SOURCE_DIR}/dep/nanogui.lib")
ELSE()
    if (LINUX)
        set(NANOGUI_LIB "${PROJECT_SOURCE_DIR}/ext/nanogui/build/libnanogui.so")
    ELSE ()
        set(NANOGUI_LIB "${PROJECT_SOURCE_DIR}/ext/nanogui/build/libnanogui.dylib")
    ENDIF()
ENDIF()

target_link_libraries(Morisson ${NANOGUI_LIB})
