#ifndef THREADSAFESTACK_H
#define THREADSAFESTACK_H

#include <stack>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class ThreadSafeStack {
private:
    std::stack<T> stack;
    mutable std::mutex mtx;
    std::condition_variable cv;

public:
    ThreadSafeStack() = default;
    
    // Disable copy operations
    ThreadSafeStack(const ThreadSafeStack&) = delete;
    ThreadSafeStack& operator=(const ThreadSafeStack&) = delete;

    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        stack.push(std::move(value));
        cv.notify_one();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !stack.empty(); });
        
        T value = std::move(stack.top());
        stack.pop();
        return value;
    }

    std::optional<T> try_pop() {
        std::lock_guard<std::mutex> lock(mtx);
        if (stack.empty()) {
            return std::nullopt;
        }
        
        T value = std::move(stack.top());
        stack.pop();
        return value;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stack.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stack.size();
    }
};

#endif