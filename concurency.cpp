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

} // namespace safecpp