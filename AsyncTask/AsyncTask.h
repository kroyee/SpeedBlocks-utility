#ifndef ASYNCTASK_H
#define ASYNCTASK_H

#include <deque>
#include <future>
#include <mutex>

namespace AsyncTask {

namespace detail {
inline std::deque<std::future<void>> futureQueue;
inline std::mutex mutex;
}  // namespace detail

template <typename F>
void add(F func) {
    std::lock_guard<std::mutex> guard(detail::mutex);
    detail::futureQueue.emplace_back(std::async(std::launch::async, func));
}

bool empty();
void check();
void exit();
}  // namespace AsyncTask

#endif
