#include <iostream>
#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <type_traits>
#include <array>
#include <numeric>
#include <algorithm>
#include <functional>
#include <memory>

struct device
{
    virtual void output() = 0;
    virtual ~device() {}
};

template <void (*action)()> /// NOTE: you can use function ptr as template parameter
struct smart_device_0 : device
{
    void output() override
    {
        (*action)();
    }
};

template <typename Command, void (Command::*action)()> /// NOTE: you can use member function ptr as template parameter
struct smart_device : device
{
    smart_device(Command *command) : cmd(command) {}

    void output() override
    {
        (cmd->*action)();
    }

private:
    Command *cmd;
};

struct hello_command
{
    void say_hello_in_english()
    {
        std::cout << "Hello, world!\n";
    }

    void say_hello_in_spanish()
    {
        std::cout << "Hola mundo!\n";
    }
};

/* the following code shows how to using string as a template parameter */

namespace string_as_template_parameter
{
    template <size_t N>
    struct string_literal
    {
        constexpr string_literal(const char (&str)[N])
        {
            std::copy_n(str, N, value);
        }

        char value[N];
    };

    template <string_literal x>
    struct foo
    {
    };
};

template <auto x> /// NOTE: In C++17, you can use auto specifier (including the auto* and auto& forms) or decltype(auto) to specify non-type template parameters!
struct foo
{
};

namespace auto_param_pack
{
    template <auto... x>
    struct foo
    { /* ... */
    };
}

int main()
{

    std::cout << " You can use function ptr as template parameter! " << std::endl;

    hello_command cmd;

    auto w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_english>>(&cmd);
    w1->output();

    auto w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_spanish>>(&cmd);
    w2->output();

    /// w1 w2 is not the same type

    static_assert(!std::is_same_v<decltype(w1), decltype(w2)>);

    {
        std::unique_ptr<device> w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_english>>(&cmd);
        w1->output();
        std::unique_ptr<device> w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_spanish>>(&cmd);
        w2->output();
        static_assert(std::is_same_v<decltype(w1), decltype(w2)>); /// w1, w2 is the same type
    }

    std::cout << " In C++17, you can use auto specifier (including the auto* and auto& forms) or decltype(auto) to specify non-type template parameters! " << std::endl;

    [[maybe_unused]] foo<42> f1; // foo<int>
    // [[maybe_unused]] foo<42.0> f2; // foo<double>, still failed in C++20 version, the version of g++ is 9.4.0 and -std=c++2a
    // foo<"42"> f3;  // error

    // {
    //     using namespace string_as_template_parameter;
    //     [[maybe_unused]] string_as_template_parameter::foo<"42"> f; /// problematic when compiling with g++ 9.4.0 and -std=c++2a
    // }

    std::cout << " In C++17, the auto specifier can also be used with a non-type template parameter pack! " << std::endl;

    {
        using namespace auto_param_pack;
        [[maybe_unused]] auto_param_pack::foo<42, false, 'x'> f;
    }
}
