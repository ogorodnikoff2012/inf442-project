cmake_minimum_required(VERSION 3.2)
project(INF442)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
else()
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -fsanitize=address")
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter")

add_executable(components_tarjan tarjan.cpp graph.cpp test_tarjan.cpp)
add_executable(statistics tarjan.cpp graph.cpp test_statistics.cpp)