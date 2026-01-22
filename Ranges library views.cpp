namespace views = std::views;
namespace ranges = std::ranges;

// flattens one level
void test_join() {
    std::vector<std::vector<int>> sensor_readings = {
        {23, 24, 25}, {45, 46, 47, 48}, {1013, 1012}, {10, 12, 11}};

    auto historics = sensor_readings | views::join;
    std::println("historics: {}", historics);

    std::array<std::array<int, 3>, 3> matrix = {
        {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};

    auto matrix_flattened = matrix | views::join;
    std::println("matrix: {}", matrix_flattened);

    std::vector<std::vector<std::string>> cities = {{"london", "liverpool"},
        {"coventry", "bristol"}};

    auto cities_flattened = cities | views::join;
    std::println("cities: {}", cities_flattened);

    std::vector<std::vector<std::vector<std::string>>> data = {
        {{"a", "b"}, {"c", "d"}}, {{"d", "e"}, {"f", "g"}}};

    auto words_1 = data | views::join;
    std::println("words_1: {}", words_1);  // = [[],[],....]

    auto words_2 = words_1 | views::join;
    std::println("words_2: {}", words_2);  // => [...]
}

void test_join_with() {
    std::vector<std::string> cities = {"london", "liverpool", "coventry",
        "bristol"};

    auto cities_flattened = cities | views::join_with(';');
    std::println("cities: {}", cities_flattened);
    std::println("cities_2: {}",
        std::string{cities_flattened.begin(), cities_flattened.end()});

    std::vector<std::vector<std::string>> data = {
        {"a", "b"}, {"c", "d"}, {"d", "e"}, {"f", "g"}};

    auto words = data | views::join_with("_");
    std::println("words: {}", words);

    std::vector<std::string> path_components = {"home", "user", "projects",
        "cpp23", "src", "main.cpp"};

    auto file_path = path_components | std::views::join_with('/');
    std::println("path: {}", file_path);
    std::println("path_2: {}", std::string{file_path.begin(), file_path.end()});
}
