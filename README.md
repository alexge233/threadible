![#Threadible](https://github.com/alexge233/threadible/blob/master/threadible.png)


A C++11 Header only class, which allows you to use a one-time thread pool in an asynchronous (Event IO) way,
by enapsulating your tasks or jobs in classes (OOP).
For the benefits of thread-pools, please read the [#WikiPedia Article](https://en.wikipedia.org/wiki/Thread_pool) which summarizes what you gain.

This is a **header-only** library, so all you need is `threadible.hpp` and the associated boost headers:

## Dependencies

* libboost headers, thread and system

## Building

There's nothing to build, just take a look at `example.cpp`
If however you wanna see the example in action, then:

```
mkdir build
cd build
cmake ..
make
./thread_pool
```

## Example

The example uses a minified and naive approach:

- each task is allocated a resource
- threadible has a set amount of available concurrent threads
- threadible has a set amount of resources/assets 

The idea is simple:

- you add tasks (either data structures or functors)
- each one of your *asset* objects handles a task
- the queue in the thread pool will execute asynchronously and in parallel each task until no tasks remain
- you can arbitrarily add more tasks when/if needed

The approach is simple; you can pre-allocate thread-specific
memory, objects or handlers, and thus parallelise execution
without worrying about sharing memory objects (you can of course still do that).

Execution uses boost's ASIO, and C++ `std::thread` so it should be
very easy to port under different platforms, provided you have a fairly recent compiler.

See the `example.cpp` for a minified demonstration.
