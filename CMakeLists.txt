cmake_minimum_required(VERSION 3.10)
project(synaptix VERSION 0.5.1)
configure_file(src/build/build.hpp.in src/build/build.hpp)

#[[ Platform checks ]]
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(LINUX TRUE)
endif ()

if (NOT LINUX)
	message( FATAL_ERROR "Fatal Error! This application is unsupported on this system!" )
endif()

#[[ Set default build target to release ]]
set(DEFAULT_BUILD_TYPE Release)
if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "The build type was not set! Defaulting to ${DEFAULT_BUILD_TYPE}")
	set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}")
endif()

#[[ Enforce expected C++ standard ]]
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#[[ Set stricter debugging flags ]]
set(PROJECT_DEBUG_FLAGS "${PROJECT_DEBUG_FLAGS} -Wall -Wextra -Werror -DPROJ_DEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

#[[ Generate assembler output ]]
if (GEN_ASSEMBLER)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -S")
endif()

#[[ Include project headers ]]
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ./src/")

#[[ Set base files to compile ]]
set(BASE_SOURCES src/*.cpp src/*/*.cpp)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
	FILE(GLOB SOURCES ${BASE_SOURCES} src/tests/*.cpp)
	set(EXECUTABLE ${CMAKE_PROJECT_NAME}-debug)
else()
	FILE(GLOB SOURCES ${BASE_SOURCES})
	set(EXECUTABLE ${CMAKE_PROJECT_NAME})
endif()

add_executable(${EXECUTABLE} ${SOURCES})
