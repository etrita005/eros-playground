#include "common.h"
#include <sdbus-c++/sdbus-c++.h>
#include <iostream>

int main() {
    auto connection = sdbus::createSessionBusConnection();
    auto proxy = sdbus::createProxy(*connection, SERVICE_NAME, OBJECT_PATH);

    std::string xml;
    proxy->callMethod("Introspect")
        .onInterface("org.freedesktop.DBus.Introspectable")
        .storeResultsTo(xml);

    std::cout << xml << std::endl;

    return 0;
}
