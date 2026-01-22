#include <iostream>
#include <reflexpr>
#include <meta>
#include <string>

template <typename T>
void print(const T& obj) {
    constexpr auto info = reflexpr(T);
    constexpr auto members = meta::get_data_members(info);

    std::cout << "{ ";
    meta::for_each(members, [&](auto m) {
        std::cout << meta::get_name(m)
                  << ": "
                  << obj.*meta::get_pointer(m)
                  << ", ";
    });
    std::cout << " }\n";
}

// Example struct to demonstrate usage
struct Person {
    std::string name;
    int age;
    double height;
};

int main() {
    Person p{"John Doe", 30, 5.9};
    print(p); // This will print: { name: John Doe, age: 30, height: 5.9,  }
    return 0;
}