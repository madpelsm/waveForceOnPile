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

include_directories(${PROJECT_SOURCE_DIR}/ext/)
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${GLM_INCLUDE_DIR})

message("source dir = ${PROJECT_SOURCE_DIR}")

file(GLOB MOR_SRC_FILES "./src/*.cpp")
file(GLOB MOR_HDR_FILES "./src/*.h")

ADD_EXECUTABLE(Morisson ${MOR_SRC_FILES} ${MOR_HDR_FILES} )
target_link_libraries(Morisson)
