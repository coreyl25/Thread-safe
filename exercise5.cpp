#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

class BoundedBuffer {
private:
    std::queue<int> buffer;
    size_t capacity;
    mutable std::mutex mtx;
    std::condition_variable not_full;
    std::condition_variable not_empty;

public:
    explicit BoundedBuffer(size_t cap) : capacity(cap) {}

    void produce(int value) {
        std::unique_lock<std::mutex> lock(mtx);
        not_full.wait(lock, [this] { return buffer.size() < capacity; });
        
        buffer.push(value);
        std::cout << "Produced: " << value << " (buffer size: " << buffer.size() << ")" << std::endl;
        
        not_empty.notify_one();
    }

    int consume() {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [this] { return !buffer.empty(); });
        
        int value = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << value << " (buffer size: " << buffer.size() << ")" << std::endl;
        
        not_full.notify_one();
        return value;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return buffer.size();
    }
};

void producer(BoundedBuffer& buffer, int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value = id * 100 + i;
        buffer.produce(value);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Producer " << id << " finished" << std::endl;
}

void consumer(BoundedBuffer& buffer, int id, int count) {
    for (int i = 0; i < count; ++i) {
        buffer.consume();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    std::cout << "Consumer " << id << " finished" << std::endl;
}

int main() {
    const size_t buffer_capacity = 5;
    const int num_producers = 2;
    const int num_consumers = 2;
    const int items_per_producer = 10;
    
    BoundedBuffer buffer(buffer_capacity);
    
    std::cout << "Starting Bounded Buffer Test (capacity: " << buffer_capacity << ")" << std::endl;
    std::cout << "Producers: " << num_producers << ", Consumers: " << num_consumers << std::endl;
    std::cout << "Items per producer: " << items_per_producer << "\n" << std::endl;
    
    std::vector<std::thread> threads;
    
    // Create producer threads
    for (int i = 0; i < num_producers; ++i) {
        threads.emplace_back(producer, std::ref(buffer), i + 1, items_per_producer);
    }
    
    // Create consumer threads
    int total_items = num_producers * items_per_producer;
    int items_per_consumer = total_items / num_consumers;
    
    for (int i = 0; i < num_consumers; ++i) {
        threads.emplace_back(consumer, std::ref(buffer), i + 1, items_per_consumer);
    }
    
    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "\nAll threads completed. Final buffer size: " << buffer.size() << std::endl;
    
    return 0;
}