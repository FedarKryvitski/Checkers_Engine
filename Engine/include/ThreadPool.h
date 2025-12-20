#ifndef THREADPOOL_H
#define THREADPOOL_H

#define NOMINMAX
#include <vector>
#include <queue>
#include <windows.h>
#include <future>
#include <functional>
#include <memory>

using std::forward, std::packaged_task, std::bind, std::future, std::bind, std::make_shared;
using std::vector, std::queue;

template<typename ReturnType>
class ThreadPool {
public:
    using task_t = std::packaged_task<ReturnType()>;

public:
    explicit ThreadPool(int threadsNum) noexcept;
    ~ThreadPool();

    template<typename F, typename ... Args>
    auto add_task(F&& f, Args&&... args) -> future<decltype(f(args...))>;

    void stop();

private:
    static DWORD WINAPI worker_thread(LPVOID param);
    bool get_task(task_t& task);

    queue<task_t> m_tasks;
    vector<HANDLE> m_threads;
    int m_threadsNum;
    CRITICAL_SECTION m_queueCriticalSection;
    CONDITION_VARIABLE m_queueCV;
    volatile bool m_end{ false };
};

template<typename ReturnType>
template<typename F, typename ...Args>
auto ThreadPool<ReturnType>::add_task(F && f, Args && ...args) -> future<decltype(f(args ...))>
{
    auto lambda_task =
    [f = std::forward<F>(f), ...args = std::forward<Args>(args)] {
        return std::invoke(f, args...);
    };

    auto task = packaged_task<ReturnType()>(std::move(lambda_task));
    future<ReturnType> result = task.get_future();
    
    EnterCriticalSection(&m_queueCriticalSection);
    m_tasks.push(std::move(task));
    LeaveCriticalSection(&m_queueCriticalSection);
    
    WakeConditionVariable(&m_queueCV);
    return result;
}

template<typename ReturnType>
ThreadPool<ReturnType>::ThreadPool(int threadsNum) noexcept : m_threadsNum(threadsNum), m_end(false) {

    InitializeCriticalSection(&m_queueCriticalSection);
    InitializeConditionVariable(&m_queueCV);
    m_threads.reserve(m_threadsNum);

    for (int i = 0; i < m_threadsNum; ++i) {
        HANDLE thread = CreateThread(
            nullptr,
            0,
            worker_thread,
            this,
            0,
            nullptr
        );
        if (thread) {
            m_threads.push_back(thread);
        }
    }
}

template<typename ReturnType>
ThreadPool<ReturnType>::~ThreadPool() {
    stop();

    for (HANDLE thread : m_threads) {
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
    }
    DeleteCriticalSection(&m_queueCriticalSection);
}

template<typename ReturnType>
void ThreadPool<ReturnType>::stop() {
    EnterCriticalSection(&m_queueCriticalSection);
    m_end = true;
    LeaveCriticalSection(&m_queueCriticalSection);

    WakeAllConditionVariable(&m_queueCV);
}

template<typename ReturnType>
DWORD WINAPI ThreadPool<ReturnType>::worker_thread(LPVOID param) {
    ThreadPool* pool = static_cast<ThreadPool*>(param);
    task_t task;

    while (true) {
        if (!pool->get_task(task)) {
            break;
        }
        task();
    }
    return 0;
}

template<typename ReturnType>
bool ThreadPool<ReturnType>::get_task(task_t& task) {
    EnterCriticalSection(&m_queueCriticalSection);

    while (m_tasks.empty() && !m_end) {
        SleepConditionVariableCS(&m_queueCV, &m_queueCriticalSection, INFINITE);
    }

    if (m_end) {
        LeaveCriticalSection(&m_queueCriticalSection);
        return false;
    }

    task = move(m_tasks.front());
    m_tasks.pop();

    LeaveCriticalSection(&m_queueCriticalSection);

    return true;
}


#endif
