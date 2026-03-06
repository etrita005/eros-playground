cmake_minimum_required(VERSION 3.10)
project(hello_cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello_cmake main.cpp)

# For cross-compilation, ensure correct link order
# Libraries should come after object files
if(CMAKE_CROSSCOMPILING)
    target_link_options(hello_cmake PRIVATE
        -B/usr/bin/aarch64-linux-gnu-
    )
    target_link_libraries(hello_cmake PRIVATE
        stdc++ gcc m
    )
endif()

install(TARGETS hello_cmake DESTINATION bin)
