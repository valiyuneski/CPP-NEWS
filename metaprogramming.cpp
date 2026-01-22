#include <gtest/gtest.h>

// target template type that will wrap the types
template <class... Ts>
struct type_list {
    using type = type_list<Ts...>;

    template <class T>
    using push_back = type_list<Ts..., T>;

    template <class T>
    using push_front = type_list<T, Ts...>;
};

template <>
struct type_list<void> {};

template <typename T, class... Ts>
struct reverse {
    using type = typename reverse<Ts...>::type::template push_back<T>;
};

template <typename T>
struct reverse<T> {
    using type = type_list<T>;
};

template <typename... Ts>
using reverse_t = typename reverse<Ts...>::type;

template <template <typename> class CT, typename T>
concept has_value_field = requires {
    { CT<T>::value } -> std::convertible_to<bool>;
};

template <template <typename> class CT, typename T, typename... Ts>
requires has_value_field<CT, T>
struct filter {
    using type = std::conditional_t<
        CT<T>::value, typename filter<CT, Ts...>::type::template push_front<T>,
        typename filter<CT, Ts...>::type>;
};

template <template <typename> class CT, typename T>
requires has_value_field<CT, T>
struct filter<CT, T> {
    using type = std::conditional_t<CT<T>::value, type_list<T>, type_list<>>;
};

template <template <typename> class CT, typename... Ts>
using filter_t = typename filter<CT, Ts...>::type;

TEST(Types, Reversing) {
    static_assert(std::same_as<type_list<char, int>, reverse_t<int, char>>);
    static_assert(
        std::same_as<type_list<char, float, int>, reverse_t<int, float, char>>);
    static_assert(std::same_as<type_list<char, float, double, int>,
        reverse_t<int, double, float, char>>);
}

TEST(Types, Filtering) {
    static_assert(
        std::same_as<type_list<int, bool>,
            filter_t<std::is_integral, int, double, float, bool>>);

    static_assert(std::same_as<type_list<>, filter_t<std::is_integral, double, float>>);

    static_assert(std::same_as<
        type_list<double, float>,
        filter_t<std::is_floating_point, int, double, float, bool>>);
}
