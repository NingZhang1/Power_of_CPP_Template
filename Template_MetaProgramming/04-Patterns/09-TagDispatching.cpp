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

// tag dispatching
// a technique to select the right function to call based on the type of the argument
// it is a kind of static polymorphism

namespace examples_iterator
{
    namespace details
    {
        template <typename Iter, typename Distance>
        void advance(Iter &it, Distance n, std::random_access_iterator_tag)
        {
            it += n;
        }

        template <typename Iter, typename Distance>
        void advance(Iter &it, Distance n, std::bidirectional_iterator_tag)
        {
            if (n > 0)
            {
                while (n--)
                    ++it;
            }
            else
            {
                while (n++)
                    --it;
            }
        }

        template <typename Iter, typename Distance>
        void advance(Iter &it, Distance n, std::input_iterator_tag)
        {
            while (n--)
            {
                ++it;
            }
        }
    }

    template <typename Iter, typename Distance>
    void advance(Iter &it, Distance n)
    {
        details::advance(it, n,
                         typename std::iterator_traits<Iter>::iterator_category{});
    }
};

// one can also use constexpr if to achieve the same thing
// or std::enable_if (SNIFAE type)
// or concepts

namespace example_concept
{
    template <std::random_access_iterator Iter, typename Distance>
    void advance(Iter &it, Distance n)
    {
        it += n;
    }

    template <std::bidirectional_iterator Iter, typename Distance>
    void advance(Iter &it, Distance n)
    {
        if (n > 0)
        {
            while (n--)
                ++it;
        }
        else
        {
            while (n++)
                --it;
        }
    }

    template <std::input_iterator Iter, typename Distance>
    void advance(Iter &it, Distance n)
    {
        while (n--)
        {
            ++it;
        }
    }
};

// here is an example of tag dispatching

int main()
{
    {
        using namespace examples_iterator;

        std::vector<int> v{1, 2, 3, 4, 5};
        auto sv = std::begin(v);
        examples_iterator::advance(sv, 2);

        std::list<int> l{1, 2, 3, 4, 5};
        auto sl = std::begin(l);
        examples_iterator::advance(sl, 2);
    }

    {
        using namespace example_concept;

        std::vector<int> v{1, 2, 3, 4, 5};
        auto sv = std::begin(v);
        example_concept::advance(sv, 2);

        std::list<int> l{1, 2, 3, 4, 5};
        auto sl = std::begin(l);
        example_concept::advance(sl, 2);
    }
}