#include <optional>
#include <print>
#include <type_traits>
#include <variant>

template <typename U, typename... Ts>
concept NoneOf = not(std::same_as<U, Ts> && ...);

template <typename U, typename... Ts>
concept AllOf = (std::same_as<U, Ts> && ...);

template <typename U, typename... Ts>
concept AnyOf = (std::same_as<U, Ts> || ...);

template <typename... Ts>
  requires NoneOf<std::monostate, Ts...>
struct Node {
  using type = std::variant<std::monostate, Ts...>;
  type m_data;

  constexpr Node() = default;

  template <typename U>
    requires(std::constructible_from<type, U>)
  constexpr Node(U&& value) : m_data{std::forward<U>(value)} {}

  template <typename U, typename... Args>
    requires(std::constructible_from<U, Args...> && AnyOf<U, Ts...>)
  constexpr U& emplace(Args&&... args) noexcept {
    return m_data.template emplace<U>(std::forward<Args>(args)...);
  }

  template <typename U>
  constexpr auto get(this auto&& self) -> decltype(auto) {
    return std::get<U>(self.m_data);
  }

  template <typename U>
  constexpr auto get_if(this auto&& self) noexcept -> decltype(auto) {
    return std::get_if<U>(&self.m_data);
  }

  template <typename U>
  constexpr auto try_get(this auto&& self) noexcept -> std::optional<U> {
    if (auto ptr = std::get_if<U>(&self.m_data); ptr) {
      return *ptr;
    }
    return std::nullopt;
  }

  template <typename Self>
  constexpr bool isEmpty(this Self&& self) noexcept {
    return std::holds_alternative<std::monostate>(
      std::forward_like<Self>(self.m_data));
  }

  template <typename U>
    requires(std::convertible_to<U, Ts> || ...)
  constexpr Node& operator=(U&& value) {
    m_data = std::forward<U>(value);
    return *this;
  }

  template <typename Visitor>
  constexpr decltype(auto) visit(this auto&& self,
    Visitor&& visitor) noexcept {
    return std::visit(std::forward<Visitor>(visitor), self.m_data);
  }

  constexpr void reset(this auto&& self) noexcept {
    self.m_data = std::monostate{};
  }
};

int main() {
  Node<std::string, float, int> mNode;

  std::println("empty: {}", mNode.isEmpty());

  mNode = 3.4f;
  std::println("empty: {}", mNode.isEmpty());

  if (auto ptr = mNode.get_if<float>(); ptr) {
    std::println("{}", *ptr);
  }

  if (auto ptr = mNode.get_if<int>(); ptr) {
    std::println("{}", *ptr);
  }

  if (auto opt = mNode.try_get<int>(); opt) {
    std::println("{}", *opt);
  }
}
