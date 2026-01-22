namespace views = std::views;
namespace ranges = std::ranges;

class Store {
  std::vector<int> m_data;

 public:
  Store() = default;

  void generate() {
    m_data = views::iota(0, 100) | ranges::to<std::vector>();
  }

  // instead of using classic const T& => move to views
  // no - copy & move
  // lazy
  // no - abstractions
  // no - owning
  auto get_even_numbers() const {
    return m_data | views::filter([](int x) { return x % 2 == 0; });
  }

  ranges::view auto get_odd_numbers() const {
    return m_data | views::filter([](int x) { return x % 2 != 0; });
  }

  // verbose ? don't try => it's very complicated
  ranges::view auto get_large_numbers() const {
    return m_data | views::filter([](int x) { return x > 50; });
  }

  auto get_all() const { return views::all(m_data); }

  auto get_transformed_data() const {
    return m_data | views::filter([](auto x) { return x > 50; }) |
           views::transform([](int x) { return x * 0.1f; });
  }

  auto get_subset(size_t start, size_t count) const {
    return ranges::subrange(
        m_data.begin() + start,
        m_data.begin() + ranges::min(start + count, m_data.size()));
  }

  template <typename FN>
  auto get_matching_records(FN&& fn) const {
    if (m_data.empty()) {
      return views::empty<int>;  // empty is view so it's a functor ( not
                                 // function => !())
    }
    return m_data | views::filter(std::forward<FN>(fn));
  }
};

void test_storage() {
  Store storage{};
  storage.generate();

  std::println("even nums: {}", storage.get_even_numbers());
  std::println("odd nums: {}", storage.get_odd_numbers());
  std::println("large nums: {}", storage.get_large_numbers());
  std::println("transformed nums: {}", storage.get_transformed_data());
  std::println("ranged [1-6]: {}", storage.get_subset(1, 6));
  std::println("all: {}", storage.get_all());
}
