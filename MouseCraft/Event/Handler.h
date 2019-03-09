#pragma once

#include <functional>
#include <vector>
#include "Subject.h"
#include "Observer.h"

/// <summary>
/// A special observer that calls a given member function
/// and automatically detaches itself when deleted. 
/// </summary>
template<typename Invoker, typename ... Args>
class Handler : public Observer<Args...>
{
public:
	Handler(Invoker* i, void(Invoker::* func)(Args...)) 
	{
		_invoker = i;
		_callback = func;
	};
	
	~Handler() 
	{
		for (auto& s : _subjects)
		{
			s->Detach(this);
		}
	};
	
	virtual void Publish(Args... args) override 
	{
		(_invoker->*_callback)(args...);
	};

	// Observe the specified subject.
	void Observe(Subject<Args...>& subject)
	{
		subject.Attach(this);
		_subjects.push_back(&subject);
	}

private: 
	// Function pointer named Callback that takes in Args... and returns void.
	void (Invoker::* _callback)(Args...);

	// Pointer to the invoker (object that will call the function above)
	Invoker* _invoker;

	// List of all subjects this handler is observing. Used to detach on dtor. 
	std::vector<Subject<Args...>*> _subjects;
};

/*
Note:
Cannot use std::function and std::bind due to use of placeholders (up to 4) and not Args...
*/

