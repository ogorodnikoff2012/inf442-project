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

include_directories(src)

set(LIBRARY_SOURCE
        src/graph/graph.cpp
        src/graph/graph.h
        src/util/progress_bar.cpp
        src/util/progress_bar.h
        src/graph/random_graph.cpp
        src/graph/random_graph.h
        src/stats/statistics.cpp
        src/stats/statistics.h
        src/graph/tarjan.cpp
        src/graph/tarjan.h
        src/util/util.h
        )

add_library(common STATIC ${LIBRARY_SOURCE})

foreach (app find_connected_components random_graph_statistics demo_progress_bar)
    add_executable(${app} src/app/${app}.cpp)
    target_link_libraries(${app} PUBLIC common)
endforeach ()