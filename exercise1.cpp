#include <iostream>
#include <thread>
#include <vector>
#include "ThreadSafeQueue.h"

void producer(ThreadSafeQueue<int>& queue, int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value = id * 1000 + i;
        queue.push(value);
        std::cout << "Producer " << id << " pushed: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(ThreadSafeQueue<int>& queue, int id, int count) {
    for (int i = 0; i < count; ++i) {
        auto value = queue.pop();
        if (value) {
            std::cout << "Consumer " << id << " popped: " << *value << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main() {
    ThreadSafeQueue<int> queue;
    const int num_producers = 3;
    const int num_consumers = 2;
    const int items_per_producer = 5;
    
    std::vector<std::thread> threads;
    
    // Create producer threads
    for (int i = 0; i < num_producers; ++i) {
        threads.emplace_back(producer, std::ref(queue), i, items_per_producer);
    }
    
    // Create consumer threads
    int total_items = num_producers * items_per_producer;
    int items_per_consumer = total_items / num_consumers;
    
    for (int i = 0; i < num_consumers; ++i) {
        int items = (i == num_consumers - 1) ? (total_items - i * items_per_consumer) : items_per_consumer;
        threads.emplace_back(consumer, std::ref(queue), i, items);
    }
    
    // Wait for threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "\nAll threads completed. Queue size: " << queue.size() << std::endl;
    
    return 0;
}