#ifndef THREADIBLE_HPP
#define THREADIBLE_HPP
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include <condition_variable>
#include <vector>
/**
 * @brief simple thread pool class using boost ASIO for C++11
 * @version 0.1.0
 * @date 14-11-2017
 * @author Alex Gkiokas
 *
 * @note template param S is a resource type, which **must** have
 *       a public `std::mutex` accessible, 
 *       and it **must** expose a `run(T callback, Args... parameters)`
 *
 * @see example.cpp on how to implement the `template class S`
 */
template <class S>
class threadible
{
public:
    /// @brief construct with @param max_threads
    ///        being the maximum amount of parallel/concurrent
    ///        threads used to process your submited tasks.
    ///
    threadible(unsigned int max_threads);

    /// wait for a free slot in the pool
    void wait();

    /// stop pool thread
    void stop();

    /// \brief submit new task
    template <class F,
              typename... Args>
    void submit(F&& arg, Args... args);
    
protected:
    // process thread
    void thread_proc();
    // reduce worker pool
    void reduce();
private:
    boost::asio::io_service _io_service;
    boost::asio::io_service::work _work {_io_service};
    std::vector<std::thread> _threads;
    std::vector<std::unique_ptr<S>> _assets;
    std::condition_variable _cv;
    std::mutex _mtx;
    std::size_t _tasks = 0;
};

/**
 * Template implementations - Hackaway :-D
 */
template <class S>
threadible<S>::threadible(unsigned int max_threads)
{
    for (auto i = 0; i < max_threads; i++) {
        _threads.emplace_back(std::bind(&threadible::thread_proc, this));
        _assets.emplace_back(std::make_unique<S>(i));
    }
}

template <class S>
void threadible<S>::wait()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.wait(lock, [this] { return _tasks == 0; });
}

template <class S>
void threadible<S>::stop()
{
    wait();
    _io_service.stop();
    for (auto & t : _threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    _threads.clear();
    _assets.clear();
}

template <class S>
void threadible<S>::thread_proc()
{
    while (!_io_service.stopped()) {
        try {
            _io_service.run();
        }
        catch(const std::exception& e)  {
            printf("%s, \n", e.what());
        }
    }
}

template <class S>
void threadible<S>::reduce()
{
    // scoped unique mutex lock
    std::unique_lock<std::mutex> lock(_mtx);
    // reduce tasks - if zero unlock and notify
    if (--_tasks == 0) {
        lock.unlock();
        _cv.notify_all();
    }
}

template <class S>
template <class F,
          typename... Args>
void threadible<S>::submit(F&& arg, Args... args)
{
    std::unique_lock<std::mutex> lock(_mtx);
    ++ _tasks;
    lock.unlock();
    // post a task, once it has run, reduce tasks
    _io_service.post(
            [this, arg, args...]() mutable
            {
                for (auto & asset : _assets) {
                    if (asset->mutex.try_lock()) {
                        asset->operator()(arg, std::forward<Args>(args)...);
                        reduce();
                        asset->mutex.unlock();
                        return;
                    }
                 }
                 // if loop reaches here and hasn't executed
                 // it means we didn't find a free asset to use
                 // in which case we have a serious problem
                 if (true) throw std::runtime_error("failed to acquire asset");
            });
}
#endif
