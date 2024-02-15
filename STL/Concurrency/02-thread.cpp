//  thread.cpp
//  as of 2022-02-22 bw [bw.org]

// #include <format>
#include <iostream>
#include <chrono>
#include <thread>

using std::cout;
// using std::format;
using std::thread;

void sleepms(const unsigned ms)
{
    using std::chrono::milliseconds;
    std::this_thread::sleep_for(milliseconds(ms));
}   

void fthread(const int n)
{
    // cout << format("This is t{}\n", n);
    printf("This is t%d\n", n);

    for (size_t i{}; i < 5; ++i)
    {
        sleepms(100 * n);
        // cout << format("t{}: {}\n", n, i + 1);
        printf("t%d: %lu\n", n, i + 1);
    }
    // cout << format("Finishing t{}\n", n);
    printf("Finishing t%d\n", n);
}

int main()
{
    thread t1(fthread, 1);
    thread t2(fthread, 2);
    
    /// one way to use detach() is to let the threads run to completion
    t1.detach();
    t2.detach();
    cout << "main() sleep 2 sec\n";
    sleepms(2000); // sleep for 2 seconds, you have to give the time to finish the threads

    /// the other way is to use join() to wait for the threads to finish
    // t1.join();
    // t2.join();

    cout << "end of main()\n";
}
