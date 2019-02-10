#include "TaskScheduler.h"
#include <Windows.h>
//
//TaskScheduler::TaskScheduler()
//{
//}
//
//
//TaskScheduler::~TaskScheduler()
//{
//}

TaskScheduler& TaskScheduler::instance()
{
	static TaskScheduler singleton;
	return singleton;
}

TaskScheduler::TaskScheduler()
{
	unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
	threads.reserve(concurentThreadsSupported);
	for (int i = 0; i < concurentThreadsSupported; i++)
	{
		// std::cout << "Created thread" << std::endl;
		threads.push_back(std::thread(&TaskScheduler::ProcessTask, this));
		// threads.emplace_back(std::bind(ProcessTask));
	}

	//std::vector<std::thread> threads;
}

TaskScheduler::~TaskScheduler()
{
}

void TaskScheduler::ScheduleTask(Task * task)
{
	// LOCK
	std::unique_lock<std::mutex> lock(mtx);
	//std::cout << "+++Added task" << std::endl;
	++runningTask;
	tasks.push(task);
	cv.notify_one();
	// UNLOCK
}

Task * TaskScheduler::Retrieve()
{
	{
		// LOCK
		//std::unique_lock<std::mutex> lock(mtx);
		//cv.wait(lock);	// UNLOCK

		//if (tasks.empty())
		//{
		//	std::cout << "Waiting for tasks" << std::endl;
		//	// LOCK 
		//	std::cout << "---Found a task!" << std::endl;
		//}
		//else
		//{
		//	std::cout << "---Task was present" << std::endl;
		//}

		std::unique_lock<std::mutex> lock(mtx);
		if (!tasks.empty())
		{
			auto t = tasks.front();
			tasks.pop();
			return t;
		}
		else
		{
			return nullptr;
		}
	}
}

void TaskScheduler::ProcessTask()
{
	// std::cout << "thread initialized: " << std::this_thread::get_id() << std::endl;
	while (true)
	{
		// retrieve task 
		std::unique_lock<std::mutex> lock(mtx);
		
		while (tasks.empty()) cv.wait(lock);
		auto t = tasks.front();
		tasks.pop();

		lock.unlock();

		// execute task 
		// std::cout << std::this_thread::get_id() << std::endl;
		t->Execute();
		delete(t);
		--runningTask;
		
		waitCv.notify_one();
	}
}

void TaskScheduler::Wait()
{
	while (runningTask > 0)
	{
		std::unique_lock<std::mutex> lock(waitMtx);
		waitCv.wait(lock);
	}
}
