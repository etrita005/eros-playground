// Hello World Example using Cytoskeleton Message Queue
// Demonstrates PostHandler usage

#include <chrono>
#include <iostream>

#include "cytoskeleton/itc/message_queue/looper.h"

using namespace com::etrita::eros::cytos::itc::message_queue;

int main() {
  std::cout << "=== Hello Cytoskeleton C++ ===" << std::endl;

  // Create a looper for message processing
  auto looper = std::make_shared<Looper>("HelloLooper");

  // Post a lambda handler to print hello world
  looper->PostHandler([]() {
    std::cout << "Hello World from Cytoskeleton MessageQueue!" << std::endl;
  });

  // Post a delayed lambda handler (1 second delay)
  looper->PostHandler([]() {
    std::cout << "Delayed hello after 1 second!" << std::endl;
  }, std::chrono::milliseconds(1000));

  // Post a lambda handler to exit
  looper->PostHandler([&looper]() {
    std::cout << "Exiting..." << std::endl;
    looper->Exit();
  }, std::chrono::milliseconds(1500));

  // Start the looper and process messages
  looper->Loop();

  std::cout << "=== Done ===" << std::endl;

  return 0;
}
