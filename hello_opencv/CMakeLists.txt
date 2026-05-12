cmake_minimum_required(VERSION 3.16)

project(hello_opencv VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find dependencies installed by Conan
find_package(OpenCV REQUIRED)

# Add executable
add_executable(hello_opencv main.cpp)
target_link_libraries(hello_opencv PRIVATE opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui)

add_executable(bench_harris bench_harris.cpp)
target_link_libraries(bench_harris PRIVATE opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui)

install(TARGETS hello_opencv bench_harris
    RUNTIME DESTINATION bin
)
