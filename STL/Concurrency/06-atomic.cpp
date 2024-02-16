//  atomic.cpp
//  as of 2022-03-03 bw [bw.org]

// #include <format>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <atomic>

using std::cout;
// using std::format;
using std::string;
using std::vector;

std::atomic<bool> ready{};
std::atomic<uint64_t> g_count{};
std::atomic_flag winner{};

constexpr int max_count{1000 * 1000};
constexpr int max_threads{100};

string make_commas(const uint64_t &num)
{
    string s{std::to_string(num)};
    for (long l = s.length() - 3; l > 0; l -= 3)
    {
        s.insert(l, ",");
    }
    return s;
}

void countem(int id)
{
    while (!ready)                 // used to synchronize the threads
        std::this_thread::yield(); // yield() is a hint to the scheduler that the current thread is willing to yield its current time slice, allowing other threads to run
    for (int i{}; i < max_count; ++i)
        ++g_count;                 // atomic operation
    if (!winner.test_and_set())    // it sets the flag and returns bool value from before it is set. 
    {
        // std::cout << format("thread {:02} won!\n", id);
        printf("thread %02d won!\n", id);
    }
};

int main()
{
    vector<std::thread> swarm;
    // cout << format("spawn {} threads\n", max_threads);
    printf("spawn %d threads\n", max_threads);
    for (int i{}; i < max_threads; ++i)
    {
        swarm.emplace_back(countem, i);
    }
    ready = true; // all the thread starts here! 
    for (auto &t : swarm)
        t.join();
    // cout << format("global count: {}\n", make_commas(g_count));
    printf("global count: %s\n", make_commas(g_count).c_str());
    // check whether lock free
    printf("atomic is lock free: %s\n", g_count.is_lock_free() ? "yes" : "no");
    return 0;
}
