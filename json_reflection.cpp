
Valentin Yuneski <valiyuneski@gmail.com>
5:50 AM (2 hours ago)
to me

#include <reflection> // The future C++26 standard
#include <print>
#include <string_view>

template <typename T>
T parse_json(std::string_view raw) {
    T obj;

    // Iterate over fields at COMPILE TIME 😱
    template for (constexpr auto field : std::meta::members_of(^T)) {
        constexpr auto name = std::meta::name_of(field);

        if (auto val = lookup(raw, name)) {
            obj.[:field:] = *val; // "splice" the value into the field
        }
    }

    return obj;
}

// Usage: In code it looks like magic:
auto user = parse_json<User>(R"({"id": 42, "name": "Neo"})");
std::print("User: {}, ID: {}", user.name, user.id); // User: Neo, ID: 42 