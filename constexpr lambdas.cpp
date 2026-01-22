constexpr auto mul = [](auto a, auto b) constexpr { return a * b; };

constexpr auto max = [](typename T)(T a, T b) constexpr
  requires requires(T x, T y) {
    x > y;
    x < y;
    x == y;
    x != y;
  }
{ return a > b ? a : b; };

constexpr auto mul_integral = [](auto x, auto y)
  requires std::integral<decltype(x)> and std::integral<decltype(y)>
{ return x * y; };

constexpr auto add_integral = []<std::integral T>(T x, T y) constexpr {
  return x + y;
};

constexpr auto to_upper = []<std::size_t N>(const char (&arr)[N]) {
  std::array<char, N> result{};
  for (std::size_t i = 0; i < N; i++) {
    result[i] =
      (arr[i] >= 'a' and arr[i] <= 'z') ? arr[i] - ('a' - 'A') : arr[i];
  }
  return result;
};

constexpr auto fact = [](this auto self, int n) -> decltype(n) {
  return n <= 1 ? 1 : n * self(n - 1);
};

constexpr auto fib = [](this auto self, int n) -> int {
  if (n == 0) return 0;
  if (n == 1) return 1;
  return self(n - 1) + self(n - 2);
};

constexpr auto trans = []<typename T>(std::array<T, 3> const& arr,
  auto&& fn) -> decltype(auto) {
  std::array<T, 3> result;
  for (auto i = 0; i < 3; i++) {
    result[i] = fn(arr[i]);
  }
  return result;
};

int main() {
  static_assert(mul(2, 2) == 4);
  static_assert(max(2, 3) == 3);
  static_assert(mul_integral(3, 4) == 12);
  static_assert(add_integral(3, 4) == 7);

  constexpr char data[] = "hakan";
  constexpr auto name = std::to_array("HAKAN");
  static_assert(to_upper(data) == name);

  std::println("{}", std::to_array({1, 2, 3, 4}));
  std::println("{}", std::to_array("hakan"));

  static_assert(fact(5) == 120);
  static_assert(fib(10) == 55);

  constexpr auto arr = std::array{1, 2, 3};
  constexpr auto square = [](auto val) { return val * val; };
  constexpr auto squared = trans(arr, square);
  std::println("{}", squared);
}
