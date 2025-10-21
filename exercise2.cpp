#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>

// Non-thread-safe queue
std::queue<int> unsafe_queue;
std::mutex queue_mutex;

void unsafe_producer(int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value = id * 1000 + i;
        unsafe_queue.push(value);
        std::cout << "Unsafe Producer " << id << " pushed: " << value 
                  << " (size: " << unsafe_queue.size() << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void unsafe_consumer(int id, int count) {
    for (int i = 0; i < count; ++i) {
        if (!unsafe_queue.empty()) {
            int value = unsafe_queue.front();
            unsafe_queue.pop();
            std::cout << "Unsafe Consumer " << id << " popped: " << value 
                      << " (size: " << unsafe_queue.size() << ")" << std::endl;
        } else {
            std::cout << "Unsafe Consumer " << id << " found empty queue!" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }
}

void safe_producer(int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value = id * 1000 + i;
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            unsafe_queue.push(value);
            std::cout << "Safe Producer " << id << " pushed: " << value 
                      << " (size: " << unsafe_queue.size() << ")" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void safe_consumer(int id, int count) {
    for (int i = 0; i < count; ++i) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (!unsafe_queue.empty()) {
            int value = unsafe_queue.front();
            unsafe_queue.pop();
            std::cout << "Safe Consumer " << id << " popped: " << value 
                      << " (size: " << unsafe_queue.size() << ")" << std::endl;
        } else {
            std::cout << "Safe Consumer " << id << " found empty queue!" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }
}

void run_unsafe_test() {
    std::cout << "\n=== UNSAFE TEST (Race Conditions Expected) ===" << std::endl;
    unsafe_queue = std::queue<int>(); // Reset queue
    
    std::vector<std::thread> threads;
    threads.emplace_back(unsafe_producer, 1, 10);
    threads.emplace_back(unsafe_producer, 2, 10);
    threads.emplace_back(unsafe_consumer, 1, 10);
    threads.emplace_back(unsafe_consumer, 2, 10);
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "Unsafe test completed. Remaining items: " << unsafe_queue.size() << std::endl;
}

void run_safe_test() {
    std::cout << "\n=== SAFE TEST (Using Mutex) ===" << std::endl;
    unsafe_queue = std::queue<int>(); // Reset queue
    
    std::vector<std::thread> threads;
    threads.emplace_back(safe_producer, 1, 10);
    threads.emplace_back(safe_producer, 2, 10);
    threads.emplace_back(safe_consumer, 1, 10);
    threads.emplace_back(safe_consumer, 2, 10);
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "Safe test completed. Remaining items: " << unsafe_queue.size() << std::endl;
}

int main() {
    std::cout << "Demonstrating Race Conditions in Concurrent Queue Access\n" << std::endl;
    
    run_unsafe_test();
    run_safe_test();
    
    return 0;
}