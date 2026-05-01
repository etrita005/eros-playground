#include "common.h"
#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <string>

int main() {
    auto connection = sdbus::createSessionBusConnection();
    connection->requestName(SERVICE_NAME);

    auto object = sdbus::createObject(*connection, OBJECT_PATH);

    object->registerMethod("concatenate")
        .onInterface(INTERFACE_NAME)
        .withInputParamNames("a", "b")
        .withOutputParamNames("result")
        .implementedAs([](const std::string& a, const std::string& b) {
            return a + b;
        });

    std::string name = "default";
    object->registerProperty("name")
        .onInterface(INTERFACE_NAME)
        .withGetter([&name]() {
            return name;
        })
        .withSetter([&name](const std::string& value) {
            name = value;
        });

    object->registerProperty("version")
        .onInterface(INTERFACE_NAME)
        .withGetter([]() {
            return std::string("1.0");
        });

    object->registerSignal("dataReady")
        .onInterface(INTERFACE_NAME)
        .withParameters<uint64_t, std::string>();

    object->finishRegistration();

    std::cout << "Server running on " << SERVICE_NAME << std::endl;
    connection->enterEventLoop();

    return 0;
}
