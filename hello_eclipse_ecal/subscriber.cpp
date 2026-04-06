#include <ecal/ecal.h>
#include <iostream>
#include <string>
#include <atomic>
#include <thread>
#include <chrono>

std::atomic<bool> g_running(true);

void OnMessage(const eCAL::STopicId& publisher_id, const eCAL::SDataTypeInformation& data_type_info, const eCAL::SReceiveCallbackData& data) {
    std::string msg(static_cast<const char*>(data.buffer), data.buffer_size);
    std::cout << "Received from topic '" << publisher_id.topic_name << "': " << msg << std::endl;
}

int main() {
    // Initialize eCAL
    eCAL::Initialize("subscriber");
    
    // Create a subscriber
    eCAL::CSubscriber subscriber("hello_topic");
    
    // Set callback
    subscriber.SetReceiveCallback(OnMessage);
    
    std::cout << "Subscriber started. Press Enter to stop..." << std::endl;
    
    // Wait for messages
    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Check for exit
        if (std::cin.rdbuf()->in_avail() > 0) {
            break;
        }
    }
    
    // Finalize eCAL
    eCAL::Finalize();
    
    return 0;
}
