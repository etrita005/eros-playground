#include "common.h"
#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    auto connection = sdbus::createSessionBusConnection();
    auto proxy = sdbus::createProxy(*connection, SERVICE_NAME, OBJECT_PATH);

    proxy->uponSignal("dataReady")
        .onInterface(INTERFACE_NAME)
        .call([](uint64_t timestamp, const std::string& message) {
            std::cout << "Received signal: timestamp=" << timestamp
                      << ", message=" << message << std::endl;
        });

    std::cout << "Signal receiver started, waiting for signals..." << std::endl;

    connection->enterEventLoopAsync();
    std::this_thread::sleep_for(std::chrono::seconds(15));

    return 0;
}
