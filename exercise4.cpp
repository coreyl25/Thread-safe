#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>

template<typename T>
class ThreadSafeLinkedList {
private:
    std::list<T> list;
    mutable std::mutex mtx;

public:
    void push_front(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        list.push_front(std::move(value));
    }

    void push_back(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        list.push_back(std::move(value));
    }

    bool pop_front(T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (list.empty()) {
            return false;
        }
        value = std::move(list.front());
        list.pop_front();
        return true;
    }

    bool pop_back(T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (list.empty()) {
            return false;
        }
        value = std::move(list.back());
        list.pop_back();
        return true;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return list.size();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return list.empty();
    }

    void print() const {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "List contents: ";
        for (const auto& item : list) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

void front_inserter(ThreadSafeLinkedList<int>& list, int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value = id * 1000 + i;
        list.push_front(value);
        std::cout << "Thread " << id << " inserted at front: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void back_inserter(ThreadSafeLinkedList<int>& list, int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value = id * 1000 + i;
        list.push_back(value);
        std::cout << "Thread " << id << " inserted at back: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void front_remover(ThreadSafeLinkedList<int>& list, int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value;
        if (list.pop_front(value)) {
            std::cout << "Thread " << id << " removed from front: " << value << std::endl;
        } else {
            std::cout << "Thread " << id << " found empty list (front)" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

void back_remover(ThreadSafeLinkedList<int>& list, int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value;
        if (list.pop_back(value)) {
            std::cout << "Thread " << id << " removed from back: " << value << std::endl;
        } else {
            std::cout << "Thread " << id << " found empty list (back)" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main() {
    ThreadSafeLinkedList<int> list;
    
    std::vector<std::thread> threads;
    
    // Create threads that insert and remove from both ends
    threads.emplace_back(front_inserter, std::ref(list), 1, 5);
    threads.emplace_back(back_inserter, std::ref(list), 2, 5);
    threads.emplace_back(front_remover, std::ref(list), 3, 5);
    threads.emplace_back(back_remover, std::ref(list), 4, 5);
    
    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "\nFinal list size: " << list.size() << std::endl;
    list.print();
    
    return 0;
}