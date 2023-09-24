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

template <typename Command, void (Command::*action)()> /// NOTE: you can use function ptr as template parameter
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

int main()
{

    std::cout << " You can use function ptr as template parameter! " << std::endl;

    hello_command cmd;

    auto w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_english>>(&cmd);
    w1->output();

    auto w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_spanish>>(&cmd);
    w2->output();
}