namespace views = std::views;
namespace ranges = std::ranges;

// returns subrange
void test_split() {
    const std::string text =
        "The quick brown fox jumps over the lazy dog. "
        "Machine learning models process text as tokens. "
        "This is example text for tokenization.";

    auto tokens =
        text | views::split(' ') | views::transform([](const auto& subrange) {
            if (subrange.back() == '.') {
                return std::string_view{subrange.begin(), subrange.end() - 1};
            } else {
                return std::string_view{subrange};
            }
        });

    std::println("tokens count: {}", ranges::distance(tokens));
    ranges::for_each(tokens,
        [](const auto& token) { std::println("{}", token); });

    auto sentences =
        text | views::split('.') | views::transform([](const auto& subrange) {
            return std::string_view{subrange};
        });

    std::println("senteces count: {}", ranges::distance(sentences));
    ranges::for_each(
        sentences, [](const auto& sentence) { std::println("{}", sentence); });
}

// the data is read / parsed once...
// generally used for input streaming data...split is good enough for most cases
void test_lazy_split() {
    std::istringstream data_stream(
        "user:123,query:hello world,time:2024-01-15\n"
        "user:456,query:tokenization example,time:2024-01-16\n"
        "user:789,query:C++23 ranges,time:2024-01-17\n");

    std::string line;
    while (std::getline(data_stream, line)) {
        auto pairs = line | views::lazy_split(',');
        for (const auto& pair : pairs) {
            auto key_values = pair | views::split(':');

            auto it = key_values.begin();
            auto key_range = *it++;
            auto value_range = *it;

            std::println("{} {}",
                std::string{key_range.begin(), key_range.end()},
                std::string{value_range.begin(), value_range.end()});
        }
    }
}
