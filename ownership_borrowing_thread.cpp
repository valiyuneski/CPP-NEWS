#include "safecpp/memory.hpp"
#include "safecpp/concurrency.hpp"

#include <string>
#include <thread>
#include <vector>
#include <iostream>

using namespace safecpp;

struct Stats {
    int requests = 0;
    int errors = 0;
};

void process_request(ts_value<Stats>& stats, borrow<const std::string> user) {
    stats.write([&](Stats& s) {
        ++s.requests;
        if (user.get() == "bad") {
            ++s.errors;
        }
    });
}

int main() {
    auto user = make_owner<std::string>("Ayman");
    ts_value<Stats> stats{};

    std::vector<std::thread> workers;

    for (int i = 0; i < 4; ++i) {
        workers.emplace_back([&] {
            for (int j = 0; j < 1000; ++j) {
                process_request(stats, borrow<const std::string>(*user));
            }
        });
    }

    for (auto& t : workers) t.join();

    stats.read([](const Stats& s) {
        std::cout << "requests = " << s.requests
                  << ", errors = " << s.errors << "\n";
    });
}