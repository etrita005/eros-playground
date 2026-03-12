#include <fmt/core.h>
#include "mylib.h"

int main() {
    fmt::print("Hello from Bazel + CMake + Conan!\n");
    fmt::print("{}\n", mylib::greet("World"));
    fmt::print("1 + 2 = {}\n", mylib::add(1, 2));
    return 0;
}
