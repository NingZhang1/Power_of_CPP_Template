//  multi-producer-consumer.cpp
//  as of 2022-03-06 bw [bw.org]

// #include <format>
#include <iostream>
#include <list>
#include <string>
#include <deque>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <chrono>

using std::async;
using std::condition_variable;
using std::cout;
// using std::format;
using std::future;
using std::list;
using std::mutex;
using std::string;
using std::unique_lock;

using namespace std::chrono_literals;
namespace this_thread = std::this_thread;

constexpr auto delay_time{50ms};
constexpr auto consumer_wait{100ms};
constexpr size_t queue_limit{5};
constexpr size_t num_items{150};
constexpr size_t num_producers{3};
constexpr size_t num_consumers{5};

std::deque<string> qs{};
mutex q_mutex{};
condition_variable cv_producer{};
condition_variable cv_consumer{};
bool production_complete{};

/// locks waits notification

void producer(const size_t id)
{
    for (size_t i{}; i < num_items; ++i)
    {
        this_thread::sleep_for(delay_time * id);
        unique_lock<mutex> lock(q_mutex);
        cv_producer.wait(lock, [&]
                         { return qs.size() < queue_limit; });
        /// NOTES: this conditional number is used to wait until the queue is not full and acquire the lock 
        /// The one get the lock will wake up
        // qs.push_back(format("pid {}, qs  {}, item {:02}\n", id, qs.size(), i + 1));
        char buf[80];
        snprintf(buf, sizeof(buf), "pid %zu, qs %zu, item %02zu\n", id, qs.size(), i + 1);
        qs.push_back(buf);
        cv_consumer.notify_all();
        /// if we do not use notify_all, the consumer will not wake up
        //. NOTES: notify_all() unblocks all waiting threads sharing the same condition variable
    }
}

void consumer(const size_t id)
{
    while (!production_complete)
    {
        unique_lock<mutex> lock(q_mutex);
        cv_consumer.wait_for(lock, consumer_wait, [&]
                             { return !qs.empty(); });
        /// NOTES: this conditional number is used to wait until the queue is not empty and acquire the lock 
        /// OR upto consumer_wait time, to prevent the consumer from waiting forever   
        if (!qs.empty())
        {
            // cout << format("cid {}: {}", id, qs.front());
            printf("cid %zu: %s", id, qs.front().c_str());
            qs.pop_front();
        }
        cv_producer.notify_all();
    }
}

int main()
{
    list<future<void>> producers;
    list<future<void>> consumers;

    for (size_t i{}; i < num_producers; ++i)
    {
        producers.emplace_back(async(producer, i));
    }

    for (size_t i{}; i < num_consumers; ++i)
    {
        consumers.emplace_back(async(consumer, i));
    }

    for (auto &f : producers)
        f.wait();
    production_complete = true;
    cout << "producers done.\n";

    for (auto &f : consumers)
        f.wait();
    cout << "consumers done.\n";
}
