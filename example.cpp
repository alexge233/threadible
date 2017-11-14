#include <stdio.h>
#include "threadible.hpp"
#include <chrono>

using namespace std::chrono_literals;

/**
 * just ane example task, all it does is
 * print a number and the resource ID it is using
 */
struct task
{
    const int number;

    task(int i) : number(i) {}

    int operator()(int res) const
    {
        printf("%s %d  %s %d\r\n", 
               "task #: ", number,  " resource #: ", res);
        std::this_thread::sleep_for(1s);
    }
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

    template <class Task,
              typename... Args>
    void operator()(Task arg, Args... args) const
    {
        arg(number, args...);
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
    // start executing them.
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
