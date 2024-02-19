#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <string_view>
#include <memory>
#include <functional>
#include <future>
#include <thread>
#include <chrono>
#include <concepts>
#include <any>
#include <ranges>

namespace examples_a
{
    struct building
    {
    };
};

namespace examples_b
{
    struct building : std::enable_shared_from_this<building> /// create share_ptr in a safe way
    {
    };
};

namespace examples_c
{
    struct executor
    {
        void execute(std::function<void(void)> const &task)
        {
            threads.push_back(std::thread([task]()
                                          { 
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(250ms);

            task(); }));
        }

        ~executor()
        {
            for (auto &t : threads)
                t.join();
        }

    private:
        std::vector<std::thread> threads;
    };

    struct building : std::enable_shared_from_this<building>
    {
        building() { std::cout << "building created\n"; }
        ~building() { std::cout << "building destroyed\n"; }

        void upgrade()
        {
            if (exec)
            {
                // you cannot write
                // exec->execute([self = this]() { this->do_upgrade(); });
                // you will find that the building is destroyed before the upgrade is done
                // because the ptr (bare ptr) captured by lambda does not notice the object's destruction
                // so we must capture a shared_ptr to avoid this!
                exec->execute([self = shared_from_this()]()
                              { self->do_upgrade(); });
            }
        }

        void set_executor(executor *e)
        {
            exec = e;
        }

    private:
        void do_upgrade()
        {
            std::cout << "upgrading\n";
            operational = false;

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1000ms);

            operational = true;
            std::cout << "building is functional\n";
        }

        bool operational = false;
        executor *exec = nullptr;
    };
};

int main()
{
    {
        using namespace examples_a;

        building *b = new building();

        std::shared_ptr<building> p1{b};
        // std::shared_ptr<building> p2{ b }; // bad idea, it will cause a double delete
    }

    {
        using namespace examples_b;

        building *b = new building();

        std::shared_ptr<building> p1{b};
        std::shared_ptr<building> p2{b->shared_from_this()}; // OK, bo double delete
    }

    {
        using namespace examples_c;

        executor e;
        std::shared_ptr<building> b = std::make_shared<building>();
        b->set_executor(&e);
        b->upgrade();

        std::cout << "main finished\n";
    }
}