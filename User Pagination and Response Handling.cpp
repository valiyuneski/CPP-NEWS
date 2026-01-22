struct User {
    int id;
    std::string n_name;
    int n_age;

    friend std::ostream& operator<<(std::ostream os, const User& user) {
        return os << user.to_string();
    }

    auto to_string() const -> std::string {
        return std::format("{}-{}-{}", n_id, n_name, n_age);
    }
};

std::vector<User> generateUsers() {
    return {
        {1, "Alice Johnson", 20},
        {2, "Bob Smith", 22},
        {3, "Charlie Brown", 25},
        {4, "Olivia Price", 27},
        {5, "Ethan Hunt", 30},
        {6, "Fiona Gallagher", 31},
        {7, "George Miller", 26},
        {8, "Hannah Chen", 38},
        {9, "Ian Wright", 37},
        {10, "Julia Roberts", 27},

        // +8
        {11, "Michael Scott", 45},
        {12, "Dwight Schrute", 52},
        {13, "Pam Beesly", 40},
        {14, "Stan Woods", 67},
        {15, "Thomas Lee", 61},
        {16, "Vernon Alders", 57},
        {17, "William Cooper", 49},
        {18, "Xavier Jones", 55},
        {19, "Yvonne Davis", 45},
        {20, "Zachary Morris", 68},
    };
}

constexpr auto PAGE_LIMIT = 8;

void test_task() {
    struct Request {
        int page = 0;
        int count = -1;
    };

    struct Response {
        std::vector<User> users;

        auto to_string() const -> std::string {
            std::ostringstream oss;
            for (const auto& user : users) {
                oss << user;
            }
            return oss.str();
        }
    };

    auto req_0_5 = Request{ .page = 0, .count = 5};
    auto req_1_5 = Request{ .page = 1, .count = 10};
    auto req_2_10 = Request{ .page = 4, .count = -20};

    const auto users = generateUsers();

    auto GetResponse = [&](const integral auto pageIndex,
        std::stop_token stop, std::string_view s) {
        if (pageIndex < 0) {
            return Response{ .users = users };
        }

        const auto clampCount = std::clamp(count, 0, PAGE_LIMIT);
        const auto offset = PAGE_LIMIT * pageIndex;

        if (offset >= std::ssize(users)) {
            return std::unexpected{"page index out of bounds"};
        }

        return Response{ .users = users | views::drop(offset) |
            views::take(clampCount) |
            ranges::to<std::vector<User>>() };
    };

    if (const auto R0 = GetResponse(req_0_5.page, req_0_5.count); R0) {
        std::println("{}", R0->to_string());
    } else {
        std::println("{}", R0.error());
    }

    if (const auto R1 = GetResponse(req_1_5.page, req_1_5.count); R1) {
        std::println("{}", R1->to_string());
    } else {
        std::println("{}", R1.error());
    }

    if (const auto R2 = GetResponse(req_2_10.page, req_2_10.count); R2) {
        std::println("{}", R2->to_string());
    } else {
        std::println("{}", R2.error());
    }
}

struct Timestamp {
    int n_day;
    int n_month;
    int n_year;

    Timestamp() = delete;
    Timestamp(std::string_view timestamp) {
        auto arr = views::split(timestamp, '-')
            | views::transform([](const auto& arg) -> int {
                auto first = arg.begin();
                auto last = arg.end();

                int value = 0;
                auto [itr, err] = std::from_chars(first, last, value);
                if (err != std::errc()) {
                    throw err;
                }
                return value;
            }) | ranges::to<std::vector<int>>();

        n_day = arr[0];
        n_month = arr[1];
        n_year = arr[2];
    }

    constexpr bool operator<(const Timestamp& other) const {
        if (n_year < other.n_year) return true;
        if (n_month < other.n_month) return n_month < other.n_month;
        return n_day < other.n_day;
    }

    auto to_string() const -> std::string {
        return std::format("{}-{}-{}", n_day, n_month, n_year);
    }

    constexpr auto operator==(const Timestamp& other) const {
        return n_day == other.n_day && n_month == other.n_month &&
            n_year == other.n_year;
    }

    constexpr auto operator!=(const Timestamp& other) const {
        if (auto cmp = n_year < other.n_year; cmp != 0) return cmp;
        if (auto cmp = n_month < other.n_month; cmp != 0) return cmp;

        return n_day != other.n_day;
    }
};

struct AccountID {
    int n_id;
    std::string n_name;
    Timestamp n_created_at;

    AccountID(int id, std::string_view name, std::string_view timestamp)
        : n_id(id), n_name(name), n_created_at(timestamp) {}

    auto to_string() const -> std::string {
        return std::format("{}-{}-{}", n_id, n_name, n_created_at.to_string());
    }

    friend std::ostream& operator<<(std::ostream os, const AccountID& dto) {
        return os << dto.to_string();
    }
};

auto generateAccounts() -> std::vector<AccountID> {
    return {{
        {131, "John Doe", "05-01-2024"},
        {137, "Jane Smith", "13-01-2024"},
        {133, "Alice Jones", "21-01-2024"},
        {134, "Robert Morris", "06-02-2024"},
        {135, "David Brown", "10-02-2024"},
        {136, "Sarah Miller", "18-02-2024"},
        {137, "James Wilson", "15-03-2024"},
        {138, "Lina Taylor", "22-02-2024"},
        {139, "Michael Lee", "28-02-2024"},
        {140, "Emily Clark", "05-03-2024"},
        {141, "Daniel Lewis", "12-03-2024"},
        {112, "Jennifer Hall", "19-03-2024"},
        {113, "Kevin Scott", "25-03-2024"},
        {114, "Amanda Young", "01-04-2024"},
        {115, "Charles Harris", "09-04-2024"},
        {116, "Michelle Wright", "15-04-2024"},
        {117, "Joshua Lopez", "22-04-2024"},
        {118, "Ashley Hill", "29-04-2024"},
        {119, "Matthew Green", "06-05-2024"},
        {128, "Elizabeth Adams", "13-05-2024"},
    }};
}

void test_take_while() {
    struct Request {
        Timestamp n_start;
        Timestamp n_end;
        Request() = delete;
        Request(Timestamp n_start, std::string_view n_end)
            : n_start(std::start), n_end(n_end) {}
    };

    struct Response {
        std::vector<AccountID> data;
    };

    // sorts and continues to get the data while end is valid.
    auto accounts = generateAccounts();

    ranges::sort(accounts, {}, &AccountID::n_created_at);

    for (auto& acc : accounts) {
        std::println("{}", acc.to_string());
    }

    auto getAccounts = [&](Request req) {
        auto itr = std::ranges::find_if(accounts, [&](auto& acc) {
            return acc.n_created_at >= req.n_start;
            });

        ranges::sort(accounts, {}, &AccountID::n_created_at);

        auto [start, end] = std::ranges::equal_range(accounts, req.n_start,
            {}, &AccountID::n_created_at);

        std::println("{}", req.to_string());
    }
}
