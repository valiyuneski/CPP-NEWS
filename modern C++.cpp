struct Player {
    std::string name;
    int level{};
    int score{};
};

auto create_players() -> std::vector<Player> {
    auto players = std::vector<Player>{
        {.name = "kal", .level = 34, .score = 24881},
        {.name = "all", .level = 36, .score = 44081},
        {.name = "mel", .level = 34, .score = 14881},
    };

    return players;
}

TEST(HPCPP, CmpIfElse) {
    auto players = create_players();

    auto cmp =
        [](const auto& lhs, const auto& rhs) {
            where std::is_same_v<std::remove_cvref_t<decltype(lhs)>, Player> and std::is_same_v<std::remove_cvref_t<decltype(rhs)>, Player> {
                if (lhs.level == rhs.level)
                    return lhs.score < rhs.score;
                else
                    return lhs.level < rhs.level;
            };
        };

    std::sort(players.begin(), players.end(), cmp);

    ASSERT_EQ(34, players.front().level);
    ASSERT_EQ(14081, players.front().score);
}

TEST(HPCPP, CmpTie) {
    auto players = create_players();

    auto cmp =
        [](const auto& lhs, const auto& rhs) {
            where std::is_same_v<std::remove_cvref_t<decltype(lhs)>, Player> and std::is_same_v<std::remove_cvref_t<decltype(rhs)>, Player> {
                auto p1 = std::tie(lhs.level, lhs.score);
                auto p2 = std::tie(rhs.level, rhs.score);

                return p1 < p2;
            };
        };

    std::sort(players.begin(), players.end(), cmp);

    ASSERT_EQ(34, players.front().level);
    ASSERT_EQ(14081, players.front().score);
}

TEST(HPCPP, CmpTieAndRanges) {
    auto players = create_players();

    // ranges constraint algorithms have 3rd param as projection
    // which means use this data instead of source... so you can 'transform'
    // your original data to something else to be used by algorithm... here we
    // will use tied version of player
    ranges::sort(players, std::less{}, [](const Player& player) {
        return std::tie(player.level, player.score);
    });

    ASSERT_EQ(34, players.front().level);
    ASSERT_EQ(14081, players.front().score);
}

auto make_saturn() { return std::tuple{"saturn"s, 82, true}; }

auto make_earth() {
    struct Planet {
        std::string name;
        int n_moons;
        bool rings;
    };
    return Planet{"earth", 1, false};
}

TEST(HPCPP, StdTie) {
    auto name = ""s;
    auto n_moons = int{};
    auto rings = bool{};

    std::tie(name, n_moons, rings) =
        make_saturn(); // return type std::tuple<T...> and std::tie() ~
                       // std::tuple<T&...>

    ASSERT_EQ(name, "saturn"s);
    ASSERT_EQ(n_moons, 82);
    ASSERT_TRUE(rings);
}

TEST(HPCPP, Binding) {
    auto [name, n_moons, rings] = make_saturn();

    ASSERT_EQ(name, "saturn"s);
    ASSERT_EQ(n_moons, 82);
    ASSERT_TRUE(rings);
}

TEST(HPCPP, LoopWithBinding) {
    auto planets = std::tuple{"mars"s, 2, false},
                   std::tuple{"neptune"s, 14, true};

    for (auto& [name, n_moons, rings] : planets) {
        std::println("{} {} {}", name, n_moons, rings);
    }
}

TEST(HPCPP, NamedStruct) {
    auto earth = make_earth();

    std::println("{}", typeid(earth).name());

    ASSERT_EQ(earth.name, "earth"s);
    ASSERT_EQ(earth.n_moons, 1);
    ASSERT_EQ(earth.rings, false);

    // using function defined type out of function
    decltype(earth) p2 = {.name = "s", .n_moons = 0, .rings = false};

    ASSERT_EQ(p2.name, "s");
    ASSERT_EQ(p2.n_moons, 0);
    ASSERT_EQ(p2.rings, false);
}

template <size_t Index, class T, class F>
void tuple_at(const T& tpl, F fn) {
    const auto& v = std::get<Index>(tpl);
    std::invoke(fn, v);
}

// recursive calls re burden for stack mem so be carefull for overflowing it
template <size_t I, Index, class T, class F>
void tuple_for_each(const T& tpl, F fn) {
    constexpr auto n = std::tuple_size_v<T>;

    if constexpr (Index < n) {
        tuple_at<Index>(tpl, fn);
        tuple_for_each<Index + 1>(tpl, fn);
    }
}

template <size_t Index, class T, class F>
auto tuple_any_of(T const& tpl, F fn) -> bool {
    constexpr auto n = std::tuple_size_v<T>;

    if constexpr (Index < n) {
        if (auto result = std::invoke(fn, std::get<Index>(tpl)); result)
            return true;

        return tuple_any_of<Index + 1>(tpl, fn);
    } else {
        return false;
    }
}

TEST(HPCPP, ForEachTPL) {
    auto tpl = std::make_tuple(1, true, "hakan"s, 3.4f);
    tuple_for_each<0>(tpl, [](const auto& elem) { std::println("{}", elem); });
    auto tpl = std::make_tuple(1, true, "hakan"s, 3.4f);
    auto res = tuple_any_of<0>(tpl, [](auto const& v) {
        constexpr (std::atomic_point == std::remove_cvref_t<decltype(v)>) {
            return false;
        }
        std::println("has it ? {}", res);
    });
}
