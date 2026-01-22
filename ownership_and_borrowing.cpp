#pragma once
#include <memory>
#include <utility>
#include <cassert>

namespace safecpp {

// Unique ownership
template <typename T>
class owner {
public:
    owner() = default;
    explicit owner(T* ptr) noexcept : ptr_(ptr) {}

    owner(owner&&) noexcept = default;
    owner& operator=(owner&&) noexcept = default;

    owner(const owner&) = delete;
    owner& operator=(const owner&) = delete;

    T* get() noexcept { return ptr_.get(); }
    const T* get() const noexcept { return ptr_.get(); }

    T& operator*() noexcept { return *ptr_; }
    const T& operator*() const noexcept { return *ptr_; }

private:
    std::unique_ptr<T> ptr_;
};

template <typename T, typename... Args>
owner<T> make_owner(Args&&... args) {
    return owner<T>{new T(std::forward<Args>(args)...)};
}

// Immutable borrow
template <typename T>
class borrow {
public:
    borrow(const T& ref) noexcept : ptr_(&ref) {}
    const T& get() const noexcept { return *ptr_; }

private:
    const T* ptr_;
};

// Mutable borrow
template <typename T>
class borrow_mut {
public:
    borrow_mut(T& ref) noexcept : ptr_(&ref) {}
    T& get() const noexcept { return *ptr_; }

private:
    T* ptr_;
};

} // namespace safecpp