cmake_minimum_required(VERSION 3.2)
project(INF442)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
else ()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -fsanitize=address")
endif ()
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter")

set(COMMON_SOURCE
        graph.cpp
        graph.h
        progress_bar.cpp
        progress_bar.h
        random_graph.cpp
        random_graph.h
        statistics.cpp
        statistics.h
        tarjan.cpp
        tarjan.h
        util.h
        )

add_executable(components_tarjan test_tarjan.cpp ${COMMON_SOURCE})
add_executable(statistics test_statistics.cpp ${COMMON_SOURCE})
add_executable(test_progress_bar test_progress_bar.cpp ${COMMON_SOURCE})