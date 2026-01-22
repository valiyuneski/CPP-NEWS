void test_packaged_task() {
    using namespace std::chrono_literals;

    // prep
    auto get_int = []{
        return 101;
    };

    auto get_string = []{
        return "hakan gedek";
    };

    auto get_void = [] (std::string_view arg) {
        std::println("hello {}", arg);
    };

    // init
    auto create_task = []<typename FN, typename ...Args>(FN&& fn, Args&&...args) -> std::packaged_task<std::invoke_result_t<FN, Args...>()>{
        //
        using RT = std::invoke_result_t<FN, Args...>;

        auto _cb = [fn = std::forward<FN>(fn), ...args = std::forward<Args>(args)]()mutable {
            return std::invoke(std::move(fn), std::move(args)...);
        };

        return std::packaged_task<RT()>{std::move(_cb)};
    };

    auto i_task = create_task(get_int);
    auto s_task = create_task(get_string);
    auto v_task = create_task(get_void, "hakan gedek");

    auto i_fut = i_task.get_future();
    auto s_fut = s_task.get_future();
    auto v_fut = v_task.get_future();

    i_task();
    s_task();
    v_task();

    std::println("get_int: {}", i_fut.get());
    std::println("get_string: {}", s_fut.get());
    v_fut.get();
}

int main() {
    test_packaged_task();
}
