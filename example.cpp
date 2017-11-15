#include <stdio.h>
#include "threadible.hpp"
#include <chrono>

using namespace std::chrono_literals;

/**
 * just ane example task, all it does is
 * encapsulate some data which will be handledd
 * by the `resource` later on
 */
struct task
{
    const int number;
    task(int i) : number(i) {}
};

/**
 * just an example resource, all it does is
 * forward its own resource number to the task.
 *
 * @WARNING: do note that it exposes a public `std::mutex`
 *           needed by the `threadible` class to find resources
 *
 * @WARNING: it also exposes a `void operator()()` method which we use
 *           in order to execute the actual task
 */
struct resource
{
    const int number;
    std::mutex mutex;

    resource(int i) : number(i) {}

    /// @brief do the actual work here
    void print(task & t) const
    { 
        printf("%s %d  %s %d\r\n", 
               "task #: ", t.number,  " resource #: ", number);
        std::this_thread::sleep_for(1s);
       
    }

    /// @brief expose this operator which will be called by `threadible::submit`
    template <typename... Args>
    void operator()(Args... args) const
    {
        print(args...);
    }
};


auto main() -> int
{
    // construct a thread pool with 4 concurrent threads,
    // and 4 resource assets (each for a thread)
    //
    threadible<resource> tpool(8);
    //
    // add tasks to the pool - it will automatically
    // start executing them - else it will sit idle.
    //
    for (int i = 0; i < 100; i++) {
        tpool.submit(task(i));
    }
    //
    // wait until all tasks have finished
    // then release the threads and resources
    //
    tpool.stop();
    return 0;
}
