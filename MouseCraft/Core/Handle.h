#pragma once

#include <shared_mutex>

template<typename ResourceType>
class Handle
{
public:
	Handle() {};
	~Handle() {};

	std::shared_lock<std::shared_timed_mutex> Read()
	{
		return std::move(std::shared_lock<std::shared_timed_mutex>(mtx));
	}

	std::unique_lock<std::shared_timed_mutex> Write()
	{
		return std::move(std::unique_lock<std::shared_timed_mutex>(mtx));
	}

	ResourceType* Get()
	{
		return &resource;
	}

private:
	ResourceType resource;
	std::shared_timed_mutex mtx;
};
