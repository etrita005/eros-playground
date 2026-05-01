#include "common.h"
#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    auto connection = sdbus::createSessionBusConnection();
    connection->requestName(SERVICE_NAME);

    auto object = sdbus::createObject(*connection, OBJECT_PATH);

    object->registerSignal("dataReady")
        .onInterface(INTERFACE_NAME)
        .withParameters<uint64_t, std::string>();

    object->finishRegistration();

    for (int i = 0; i < 5; ++i) {
        auto now = std::chrono::system_clock::now().time_since_epoch();
        uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();

        object->emitSignal("dataReady")
            .onInterface(INTERFACE_NAME)
            .withArguments(timestamp, std::string("message ") + std::to_string(i));

        std::cout << "Sent signal " << i << " at " << timestamp << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
