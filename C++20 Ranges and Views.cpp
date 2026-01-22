namespace views = std::views;
namespace ranges = std::ranges;

struct Product {
  uint64_t m_UUID;
  std::string m_Name;
  uint32_t m_Cost;
  uint32_t m_Reserve;

  auto to_string() const -> std::string {
    return std::format("{}-{}-{}-{}", m_UUID, m_Name, m_Cost, m_Reserve);
  }
};

struct Request {
  uint32_t m_Page_Index;
  uint32_t m_Count;
};

struct CostQuery {
  uint32_t m_Cost_Min;
  uint32_t m_Cost_Max;
};

struct Response {
  std::vector<Product> Products;
};

const auto generate_products() -> std::vector<Product> {
  return {{1001ULL, "Wireless Headphones", 120, 50},
          {1002ULL, "Smartphone Charger", 25, 200},
          {1003ULL, "Bluetooth Speaker", 80, 75},
          {1004ULL, "USB-C Cable (2m)", 15, 300},
          {1005ULL, "Laptop Stand", 45, 60},
          {1006ULL, "Mechanical Keyboard", 95, 40},
          {1007ULL, "Ergonomic Mouse", 35, 85},
          {1008ULL, "Webcam 1080p", 65, 30},
          {1009ULL, "Portable SSD 1TB", 150, 25},
          {1010ULL, "Desk Lamp (LED)", 40, 90},
          {1011ULL, "Wireless Mouse", 30, 120},
          {1012ULL, "Monitor Arm", 70, 20},
          {1013ULL, "Gaming Chair", 250, 10},
          {1014ULL, "Phone Tripod", 20, 150},
          {1015ULL, "Screen Protector Pack", 12, 400},
          {1016ULL, "Wireless Earbuds", 110, 70},
          {1017ULL, "Tablet Case", 28, 110},
          {1018ULL, "HDMI Cable (3m)", 18, 250},
          {1019ULL, "Mini Projector", 200, 15},
          {1020ULL, "Smart Light Bulb", 22, 180}};
}

void test_drop() {
  constexpr auto req_1 = Request{
      .m_Page_Index = 0,
      .m_Count = 5,
  };

  constexpr auto req_2 = Request{
      .m_Page_Index = 2,
      .m_Count = 7,
  };

  const auto products = generate_products();
  const auto skip = [](auto Index) {
    constexpr static auto PAGE_COUNT = 5;
    return Index * PAGE_COUNT;
  };

  auto filter = [&skip](const auto& products, const auto& req) {
    return products | views::drop(skip(req.m_Page_Index)) |
           views::take(req.m_Count) |
           views::transform(
               [](auto const& elem) { return elem.to_string(); }) |
           ranges::to<std::vector<std::string>>();
  };

  const auto result_1 = filter(products, req_1);
  const auto result_2 = filter(products, req_2);

  std::println("page 1");
  ranges::for_each(result_1,
                   [](auto const& elem) { std::println("{}", elem); });

  std::println("page 2");
  ranges::for_each(result_2,
                   [](auto const& elem) { std::println("{}", elem); });
}

void test_drop_while() {
  auto products = generate_products();
  // take_while & drop_while requires sorted collections
  ranges::sort(products, {}, &Product::m_Cost);

  constexpr auto query_1 = CostQuery{.m_Cost_Min = 20, .m_Cost_Max = 50};
  constexpr auto query_2 = CostQuery{.m_Cost_Min = 55, .m_Cost_Max = 75};

  const auto filter = [](const auto& products, const auto& query) {
    return products | views::drop_while([&query](const auto& elem) {
             return elem.m_Cost < query.m_Cost_Min;
           }) |
           views::take_while([&query](const auto& elem) {
             return elem.m_Cost < query.m_Cost_Max;
           }) |
           views::transform(
               [](const auto& elem) { return elem.to_string(); }) |
           ranges::to<std::vector<std::string>>();
  };

  const auto result_1 = filter(products, query_1);
  const auto result_2 = filter(products, query_2);

  std::println("query 1");
  ranges::for_each(result_1,
                   [](auto const& elem) { std::println("{}", elem); });

  std::println("query 2");
  ranges::for_each(result_2,
                   [](auto const& elem) { std::println("{}", elem); });
}
