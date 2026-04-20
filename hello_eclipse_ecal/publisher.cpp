#include <ecal/ecal.h>
#include <iostream>
#include <string>
#include <thread>

int main() {
    // Initialize eCAL
    eCAL::Initialize("publisher");
    
    // Create a publisher
    eCAL::CPublisher publisher("hello_topic");
    
    std::cout << "Publisher started. Press Enter to stop..." << std::endl;
    
    int counter = 0;
    while (true) {
        // Create message
        std::string message = "Hello eCAL! Counter: " + std::to_string(counter++);
        
        // Send message
        publisher.Send(message.c_str(), message.size());
        
        std::cout << "Sent: " << message << std::endl;
        
        // Wait 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Check for exit
        if (std::cin.rdbuf()->in_avail() > 0) {
            break;
        }
    }
    
    // Finalize eCAL
    eCAL::Finalize();
    
    return 0;
}
