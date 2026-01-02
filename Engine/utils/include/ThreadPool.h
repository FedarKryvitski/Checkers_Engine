#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

class ThreadPool final{
public:
    explicit ThreadPool(size_t threads = std::thread::hardware_concurrency()) noexcept;
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&&) noexcept = default;
    ThreadPool& operator=(ThreadPool&&) noexcept = default;

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<decltype(f(args...))>;

private:
    void worker();

private:
    using task_t = std::function<void()>;

    std::vector<std::jthread> workers_;
    std::queue<task_t> tasks_;
    
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> shutdown_requested_{false};
};

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<decltype(f(args...))>
{
    using return_type = decltype(f(args...));

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        [func = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable {
            return std::invoke(std::move(func), std::move(args)...);
        }
    );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock lock(queue_mutex_);

        tasks_.emplace([task](){ 
            (*task)(); 
        });
    }
    
    condition_.notify_one();
    return res;
}

#endif // THREADPOOL_H