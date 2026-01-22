#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int longRunningTask() {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a long-running task
    return 42;
}

int main() {
    // Execute the task asynchronously
    std::future<int> result = std::async(std::launch::async, longRunningTask);

    // Continue executing other tasks
    std::cout << "Waiting for the result..." << std::endl;

    // Wait for the result (similar to await)
    int value = result.get();
    std::cout << "Result: " << value << std::endl;

    return 0;
}