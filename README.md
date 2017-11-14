# Threadible

A C++11 thread pool header for easy pooling.
This is a **header-only** library, so all you need is `threadible.hpp`.

## Dependencies

* libboost headers, thread and system

## Building

There's nothing to build, just take a look at `example.cpp`

## Example

The example uses a minified and naive approach:

- each task is allocated a resource
- threadible has a set amount of available concurrent threads
- threadible has a set amount of resources/assets 

The idea is simple:

- you add tasks (either data structures or functors)
- each one of your *asset* objects handles a task

The approach is simple; you can pre-allocate thread-specific
memory, objects or handlers, and thus parallelise execution
without worrying about sharing memory objects (you can of course still do that).

Execution uses boost's ASIO, and C++ `std::thread` so it should be
very easy to port under different platforms, provided you have a fairly recent compiler.

**NOTE** only tested under Ubuntu 16.04
