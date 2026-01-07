#pragma once
#include "ThreadPool.h"

class ThreadPoolManager final {
private:
	ThreadPoolManager() noexcept = default;
	~ThreadPoolManager() = default;

	static ThreadPool instance_;

public:
	static ThreadPool *instance() { return &instance_; }

	ThreadPoolManager(const ThreadPoolManager &) = delete;
	ThreadPoolManager &operator=(const ThreadPoolManager &) = delete;
};