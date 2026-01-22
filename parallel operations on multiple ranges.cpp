void test_zip_and_transform() {
    // if you have multiple ranges and you want to operate them in parallel
    // this will allow you to handle them like tuple per index
    
    struct Customer {
    int m_table_id;
    std::string m_details;
    
    auto to_string() const { return std::format("{}", m_details); }
    };
    
    struct Bill {
        int m_table_id;
        int m_value;
        
        auto to_string() const {
            return std::format("{} {}", m_table_id, m_value);
        }
    };
    
    struct TransformResult {
        int m_table_id;
        int m_value;
        std::string m_details;
        auto to_string() const {
            return std::format("{} {} {}", m_table_id, m_value, m_details);
        }
    };
    
    std::vector<Customer> customers = {
        {0, "2 people;ordered...."},
        {1, "3 people;ordered...."},
        {2, "1 person; ordered..."},
    };
    
    std::vector<Bill> bills = {{0, 200}, {1, 400}, {2, 50}};
    
    auto zipped_viewed = views::zip(customers, bills);
    
    ranges::for_each(zipped_viewed, [](auto elem) {
        auto [cust, bill] = elem;
        std::println("{} {}", cust.to_string(), bill.to_string());
    });
    
    auto zipped_but_transformed = views::zip_transform(
    [](auto cust, auto bill) -> TransformResult {
        return {.m_table_id = bill.m_table_id,
        .m_value = bill.m_value,
        .m_details = cust.m_details};
        },
        customers, bills);
    
        for (auto result : zipped_but_transformed) {
            std::println("{}", result.to_string());
        }
    }
    
    void test_adjacent_and_transform() {
        // similar to views::slide
        // creates windows like it but this uses tuple
        // compile time vs runtime
        // tuple vs subrange
        
        std::vector<double> temps = {20.5, 21, 22, 20, 18, 19};
        // [20.5, 21], [21,22],...=> tuples
        
        auto avgs = temps | views::adjacent<2> | views::transform([](auto tpl) {
            return (std::get<0>(tpl) + std::get<1>(tpl)) / 2;
        });
        
        std::println("avgs => {}", avgs);
        
        auto diffs = temps | views::adjacent_transform<2>(
        [](auto t0, auto t1) { return t1 - t0; });
    }
    