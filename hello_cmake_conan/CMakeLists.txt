cmake_minimum_required(VERSION 3.16)

# EROS toolchain is already set as CMAKE_TOOLCHAIN_FILE - let it do the compiler detection
project(hello_cmake_conan VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Now include Conan toolchain for dependency finding (after project() so it doesn't mess up compiler detection)
if(DEFINED CONAN_TOOLCHAIN_FILE AND EXISTS ${CONAN_TOOLCHAIN_FILE})
    include(${CONAN_TOOLCHAIN_FILE})
endif()

# CRITICAL: Call force_toolchain_settings() if it exists (from EROS toolchain)
# This will OVERRIDE any bad flags Conan added!
if(COMMAND force_toolchain_settings)
    force_toolchain_settings()
endif()

# Find dependencies installed by Conan
find_package(fmt REQUIRED)

# Add executable
add_executable(hello main.cpp)

# Link libraries
target_link_libraries(hello PRIVATE fmt::fmt)

# Install rules
install(TARGETS hello
    RUNTIME DESTINATION bin
)
