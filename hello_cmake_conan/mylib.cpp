#include "mylib.h"
#include <fmt/format.h>

namespace mylib {

std::string greet(const std::string& name) {
    return fmt::format("Hello, {}! (from mylib)", name);
}

int add(int a, int b) {
    return a + b;
}

}
