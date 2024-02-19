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

/// Expression Templates
/// a technique to delay the evaluation of an expression
/// avoid inefficient temporary objects and function calls at runtime

namespace examples_ET
{
    template <typename T, typename C = std::vector<T>> // C is the container type, T is the type of the elements
    struct vector
    {
        vector() = default;

        vector(std::size_t const n) : data_(n) {}

        vector(std::initializer_list<T> &&l) : data_(l) {}

        vector(C const &other) : data_(other) {}

        template <typename U, typename X>
        vector(vector<U, X> const &other) : data_(other.size()) // copy constructor, the expression is evaluated here
        {
            for (std::size_t i = 0; i < other.size(); ++i)
                data_[i] = static_cast<T>(other[i]);
        }

        template <typename U, typename X>
        vector &operator=(vector<U, X> const &other) // copy assignment, the expression is evaluated here
        {
            data_.resize(other.size());
            for (std::size_t i = 0; i < other.size(); ++i)
                data_[i] = static_cast<T>(other[i]);

            return *this;
        }

        std::size_t size() const noexcept
        {
            return data_.size();
        }

        T operator[](const std::size_t i) const
        {
            return data_[i];
        }

        T &operator[](const std::size_t i)
        {
            return data_[i];
        }

        C &data() noexcept { return data_; }

        C const &data() const noexcept { return data_; }

    private:
        C data_;
    };

    template <typename L, typename R> /// template to hold the expression
    struct vector_add
    {
        vector_add(L const &a, R const &b) : lhv(a), rhv(b) {}

        auto operator[](std::size_t const i) const /// this is when the expression is evaluated
        {
            return lhv[i] + rhv[i];
        }

        std::size_t size() const noexcept
        {
            return lhv.size();
        }

    private:
        L const &lhv;
        R const &rhv;
    };

    template <typename L, typename R> /// template to hold the expression
    struct vector_mul
    {
        vector_mul(L const &a, R const &b) : lhv(a), rhv(b) {}

        auto operator[](std::size_t const i) const /// this is when the expression is evaluated
        {
            return lhv[i] * rhv[i];
        }

        std::size_t size() const noexcept
        {
            return lhv.size();
        }

    private:
        L const &lhv;
        R const &rhv;
    };

    template <typename S, typename R>
    struct vector_scalar_mul
    {
        vector_scalar_mul(S const &s, R const &b) : scalar(s), rhv(b) {}

        auto operator[](std::size_t const i) const /// this is when the expression is evaluated
        {
            return scalar * rhv[i];
        }

        std::size_t size() const noexcept
        {
            return rhv.size();
        }

    private:
        S const &scalar;
        R const &rhv;
    };

    template <typename T, typename L, typename U, typename R>
    auto operator+(vector<T, L> const &a, vector<U, R> const &b)
    {
        using result_type = decltype(std::declval<T>() + std::declval<U>());

        return vector<result_type, vector_add<L, R>>(vector_add<L, R>(a.data(), b.data()));
    }

    template <typename T, typename L, typename U, typename R>
    auto operator*(vector<T, L> const &a, vector<U, R> const &b)
    {
        using result_type = decltype(std::declval<T>() + std::declval<U>());

        return vector<result_type, vector_mul<L, R>>(vector_mul<L, R>(a.data(), b.data()));
    }

    template <typename T, typename S, typename E>
    auto operator*(S const &a, vector<T, E> const &v)
    {
        using result_type = decltype(std::declval<T>() + std::declval<S>());

        return vector<result_type, vector_scalar_mul<S, E>>(vector_scalar_mul<S, E>(a, v.data()));
    }
}

int main()
{
    examples_ET::vector<int> v1{1, 2, 3};
    examples_ET::vector<int> v2{4, 5, 6};
    double a{1.5};

    examples_ET::vector<double> v3 = v1 + a * v2; // {7.0, 9.5, 12.0}

    int c{3};
    // std::cin >> c;
    examples_ET::vector<double> v31 = v1 + c * v2;   // {7.0, 9.5, 12.0}
    examples_ET::vector<int> v4 = v1 * v2 + v1 + v2; // {9, 17, 27}
}