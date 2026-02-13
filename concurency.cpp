#pragma once
#include <mutex>
#include <functional>

namespace safecpp {

template <typename T, typename Mutex = std::mutex>
class ts_value {
public:
    template <typename... Args>
    explicit ts_value(Args&&... args)
        : value_(std::forward<Args>(args)...) {}

    template <typename F>
    auto read(F&& f) const {
        std::scoped_lock lock(m_);
        return std::invoke(std::forward<F>(f), value_);
    }

    template <typename F>
    auto write(F&& f) {
        std::scoped_lock lock(m_);
        return std::invoke(std::forward<F>(f), value_);
    }

private:
    mutable Mutex m_;
    T value_;
};

safecpp::ts_value<int> counter(0);

int main() {
    // Write: modify the value
    counter.write([](int& v) {
        v += 1;
    });

    // Read: access the value
    int value = counter.read([](const int& v) {
        return v;
    });

    std::cout << value << "\n"; // 1
}

} // namespace safecpp