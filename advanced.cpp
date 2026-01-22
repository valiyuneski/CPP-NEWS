namespace ranges = std::ranges;
namespace views = std::views;

template <typename T>
concept StringConvertible = requires(T t) {
    { t.to_string() } -> std::convertible_to<std::string>;
};

namespace {
struct stringify_fn {
    template <ranges::input_range R>
    requires StringConvertible<ranges::range_value_t<R>>
    constexpr auto operator()(R&& range) const {
        using value_type = ranges::range_value_t<R>;
        std::ostringstream oss;
        for (const auto& item : range) {
            oss << item.to_string() << ';';
        }
        return oss.str();
    }
};

struct sum_fn {
    template <ranges::input_range R, typename Init = ranges::range_value_t<R>>
    constexpr auto operator()(R&& range, Init init = {}) const {
        return std::accumulate(ranges::begin(range), ranges::end(range),
                               std::move(init));
    }
};

template <ranges::input_range R>
constexpr auto operator|(R&& range, const sum_fn& fn) {
    return fn(std::forward<R>(range));
}

template <ranges::input_range R>
constexpr auto operator|(R&& range, const stringify_fn& fn) {
    return fn(std::forward<R>(range));
}

inline constexpr sum_fn sum;
inline constexpr stringify_fn stringify;
}  // namespace

// lazy filtering
void test_filter() {
    // valid + invalid bytes
    std::string_view text =
        "fdsadfslaifdsiafdsjiafoijdspaifd;saifd;slakjj";

    // extract valid alpha-num from invalid text
    auto filtered = text | views::filter([](char c) { return isalnum(c); });
    // che
    auto hasNoInvalid =
        ranges::none_of(filtered, [](char c) { return ispunct(c); });
    auto allValid =
        ranges::all_of(filtered, [](char c) { return isalnum(c); });
    assert(hasNoInvalid);
    assert(allValid);
}

// random node data
struct Node {
    bool enabled = std::rand();
    uint32_t value = std::rand() % 71;
};

// creator
constexpr auto factory = []<size_t Index, size_t N>(
                            this auto&& self,
                            std::array<Node, N>& nodes) -> void {
    if constexpr (Index == 0)
        return;
    else {
        nodes[Index - 1] = Node{};
        return std::forward<decltype(self)>(self)
            .template operator()<Index - 1>(nodes);
    }
};

constexpr size_t N = 10;
std::array<Node, N> nodes = {};

// create
factory.operator()<N>(nodes);

// filter only enabled nodes then grab their values and sum the value
// list
auto total =
    nodes | views::filter([](auto&& node) { return node.enabled; }) |
    views::transform([](auto&& node) { return node.value; }) | sum;
std::println("total: {}", total);
}

// lazy map op which in_t => out_t
void test_transform() {
    // flatning op ? yes
    // mapping op ? yes
    // transform op ? yes ... all

    // random transfer data
    struct Transfer {
        int balance = (rand() % 51 - 20);
        std::string to_string() const { return std::to_string(balance); }
    };

    constexpr size_t N = 20;

    // random transfers
    std::vector<Transfer> transfers{};
    transfers.reserve(N);

    // creator
    auto factory = [&transfers]<size_t N>(this auto&& self) {
        if constexpr (N == 0) {
            return;
        } else {
            transfers.push_back(Transfer{});
            return std::forward<decltype(self)>(self)
                .template operator()<N - 1>();
        }
    };

    factory.operator()<N>();
    // total of the balance
    auto totalBalance =
        transfers |
        views::transform([](auto&& transfer) { return transfer.balance; }) |
        sum;

    // positive balance list
    auto positives =
        transfers |
        views::filter([](auto&& transfer) { return transfer.balance > 0; }) |
        ranges::to<std::vector<Transfer>>();

    // negative balance list
    auto negatives =
        transfers |
        views::filter([](auto&& transfer) { return transfer.balance < 0; }) |
        ranges::to<std::vector<Transfer>>();

    std::println("positives: {}", positives | stringify);
    std::println("negatives: {}", negatives | stringify);
}
