#pragma once
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <atomic>
#include "Task.h"

class TaskScheduler
{
public:
	static TaskScheduler& instance();
	TaskScheduler();
	~TaskScheduler();
	std::queue<Task*> tasks;
	std::vector<std::thread> threads;
	// CircularBuffer<std::thread> threads;
	
	std::mutex mtx;
	std::condition_variable cv;

	std::atomic<int> runningTask;
	std::mutex waitMtx;
	std::condition_variable waitCv;

public: 

	void ScheduleTask(Task* task);

	Task* Retrieve();

	void ProcessTask();

	void Wait();

};

