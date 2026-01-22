template <class T, class E>
struct expected {

    std::variant<T, E> data;

    constexpr expected (T value): data{std::move(value)} {}
    constexpr expected (E err): data{ std::move(err)} {}

    constexpr bool is_ok() const { return std::holds_alternative<T>(data); }
    constexpr bool is_err() const { return std::holds_alternative<E>(data); }

    constexpr std::optional<T> ok() const {
        if (is_ok()) return std::get<T>(data);
        return std:: nullopt;
    }

    constexpr std::optional<E> err() const {
        if (is_err()) return std::get<E>(data);
        return std:: nullopt;
    }

    constexpr operator bool() const { 
        return is_ok() ? true : false; 
    }
};

auto parse_number(std::string_view& str) -> expected<double, err> {
    char const* begin = str.data();
    char* end = nullptr;

    double retval = std::strtod(begin, &end);

    if (begin == end)
        return err::invalid_input;
    else if (std::isinf (retval))
        return err::overflow;

    str.remove_prefix(end - begin); // remove the parsed section
    return retval;
}

TEST(CPP2X, ExpectedLocal) {
    for (auto src: {"32"sv, "123hakan"sv, "hakan"sv, "inf"sv}) {
        let result = parse_number(src);
        std::println("{}", result.ok()
        .or_else([] { return std::optional<double>{0}; })
        .value());
    }
}
