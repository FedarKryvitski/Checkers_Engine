#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threads) noexcept
    : shutdown_requested_(false)
{
    for (size_t i{}; i < threads; ++i)
    {
        workers_.emplace_back([this](){
            this->worker();
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock lock(queue_mutex_);
        shutdown_requested_ = true;
    }
    condition_.notify_all();
}

void ThreadPool::worker()
{
    while (1) 
    {
        task_t task;
        {
            std::unique_lock lock(queue_mutex_);

            condition_.wait(lock, [this](){ 
                return shutdown_requested_ || !tasks_.empty(); 
            });

            if (shutdown_requested_)
                return;

            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}