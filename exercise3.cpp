#include <iostream>
#include <thread>
#include <vector>
#include "ThreadSafeQueue.h"
#include "ThreadSafeStack.h"

void queue_pusher(ThreadSafeQueue<int>& queue, int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value = id * 100 + i;
        queue.push(value);
        std::cout << "Queue Pusher " << id << " pushed: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void queue_popper(ThreadSafeQueue<int>& queue, int id, int count) {
    for (int i = 0; i < count; ++i) {
        auto value = queue.try_pop();
        if (value) {
            std::cout << "Queue Popper " << id << " popped: " << *value << std::endl;
        } else {
            std::cout << "Queue Popper " << id << " found empty queue" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }
}

void stack_pusher(ThreadSafeStack<int>& stack, int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value = id * 100 + i;
        stack.push(value);
        std::cout << "Stack Pusher " << id << " pushed: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void stack_popper(ThreadSafeStack<int>& stack, int id, int count) {
    for (int i = 0; i < count; ++i) {
        auto value = stack.try_pop();
        if (value) {
            std::cout << "Stack Popper " << id << " popped: " << *value << std::endl;
        } else {
            std::cout << "Stack Popper " << id << " found empty stack" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }
}

int main() {
    std::cout << "=== Testing Thread-Safe Queue ===" << std::endl;
    ThreadSafeQueue<int> queue;
    
    std::vector<std::thread> queue_threads;
    queue_threads.emplace_back(queue_pusher, std::ref(queue), 1, 8);
    queue_threads.emplace_back(queue_pusher, std::ref(queue), 2, 8);
    queue_threads.emplace_back(queue_popper, std::ref(queue), 1, 8);
    queue_threads.emplace_back(queue_popper, std::ref(queue), 2, 8);
    
    for (auto& thread : queue_threads) {
        thread.join();
    }
    
    std::cout << "\nQueue final size: " << queue.size() << std::endl;
    
    std::cout << "\n=== Testing Thread-Safe Stack ===" << std::endl;
    ThreadSafeStack<int> stack;
    
    std::vector<std::thread> stack_threads;
    stack_threads.emplace_back(stack_pusher, std::ref(stack), 1, 8);
    stack_threads.emplace_back(stack_pusher, std::ref(stack), 2, 8);
    stack_threads.emplace_back(stack_popper, std::ref(stack), 1, 8);
    stack_threads.emplace_back(stack_popper, std::ref(stack), 2, 8);
    
    for (auto& thread : stack_threads) {
        thread.join();
    }
    
    std::cout << "\nStack final size: " << stack.size() << std::endl;
    
    return 0;
}