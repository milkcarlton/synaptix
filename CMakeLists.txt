<<<<<<< HEAD
cmake_minimum_required(VERSION 3.2)
project(Synaptix)
=======
cmake_minimum_required(VERSION 3.10)
project(Synaptix VERSION 0.5.0)
configure_file(src/build.h.in src/build.h)
>>>>>>> 1ef07e4 (Added version flag and cmake entry)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

FILE(GLOB Sources src/*.cpp)
add_executable(synaptix ${Sources})
