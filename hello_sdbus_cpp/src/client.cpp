#include "common.h"
#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <string>

int main() {
    auto connection = sdbus::createSessionBusConnection();
    auto proxy = sdbus::createProxy(*connection, SERVICE_NAME, OBJECT_PATH);

    std::string result;
    proxy->callMethod("concatenate")
        .onInterface(INTERFACE_NAME)
        .withArguments(std::string("Hello"), std::string("World"))
        .storeResultsTo(result);

    std::cout << "concatenate result: " << result << std::endl;

    auto nameVariant = proxy->getProperty("name").onInterface(INTERFACE_NAME);
    std::string name = nameVariant.get<std::string>();
    std::cout << "name property: " << name << std::endl;

    proxy->setProperty("name").onInterface(INTERFACE_NAME).toValue(std::string("new name"));
    std::cout << "name property set to 'new name'" << std::endl;

    nameVariant = proxy->getProperty("name").onInterface(INTERFACE_NAME);
    name = nameVariant.get<std::string>();
    std::cout << "name property after set: " << name << std::endl;

    auto versionVariant = proxy->getProperty("version").onInterface(INTERFACE_NAME);
    std::string version = versionVariant.get<std::string>();
    std::cout << "version property: " << version << std::endl;

    return 0;
}
