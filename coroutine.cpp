#include <iostream>
#include <coroutine>
#include <chrono>
#include <thread>

// Define a structure for the Coroutine
struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

// Asynchronous function (Coroutine)
Task asyncTask() {
    std::cout << "Starting async task..." << std::endl;
    co_await std::suspend_always{}; // Pause execution
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a long-running task
    std::cout << "Async task completed!" << std::endl;
}

int main() {
    asyncTask(); // Start the asynchronous task
    std::cout << "Main thread continues..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait to see the result
    return 0;
}