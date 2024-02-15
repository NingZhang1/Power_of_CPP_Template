//  parallel-algorithms.cpp
//  as of 2022-02-25 bw [bw.org]

// #include <format>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <execution>
#include <chrono>

using std::cout;
// using std::format;
using std::chrono::duration;
using std::chrono::steady_clock;

namespace execution = std::execution;

using dur_t = duration<double, std::milli>;

int main()
{
    std::vector<unsigned> v(10 * 1000 * 1000);
    std::random_device rng;
    cout << "generate randoms\n";
    for (auto &i : v)
        i = rand() % 0xFFFF'FFFF;

    auto mul2 = [](int n)
    { return n * 2; };

    auto t0 = steady_clock::now();
    std::transform(v.begin(), v.end(), v.begin(), mul2);
    dur_t dur0 = steady_clock::now() - t0;
    // cout << format("no policy: {:.3}ms\n", dur0.count());
    printf("no policy: %.3fms\n", dur0.count());

    auto t1 = steady_clock::now();
    std::transform(execution::seq, v.begin(), v.end(), v.begin(), mul2);
    dur_t dur1 = steady_clock::now() - t1;
    // cout << format("execution::seq: {:.3}ms\n", dur1.count());
    printf("execution::seq: %.3fms\n", dur1.count());

    auto t2 = steady_clock::now();
    std::transform(execution::par, v.begin(), v.end(), v.begin(), mul2);
    dur_t dur2 = steady_clock::now() - t2;
    // cout << format("execution::par: {:.3}ms\n", dur2.count());
    printf("execution::par: %.3fms\n", dur2.count());

    auto t3 = steady_clock::now();
    std::transform(execution::par_unseq, v.begin(), v.end(), v.begin(), mul2);
    dur_t dur3 = steady_clock::now() - t3;
    // cout << format("execution::par_unseq: {:.3}ms\n", dur3.count());
    printf("execution::par_unseq: %.3fms\n", dur3.count());
}
