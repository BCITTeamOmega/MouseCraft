#pragma once

#include <vector>
#include <algorithm>
#include "Observer.h"

/// <summary>
/// Implementation of the observer pattern. 
/// Tracks all observers and notifies them with specified arguments.
/// </summary>
template<typename ... Args>
class Subject
{
public:
	Subject() {};
	~Subject() {};

	void Attach(Observer<Args...>& obs)
	{
		_observers.push_back(&obs);
	};

	void Detach(Observer<Args...>& obs)
	{
		auto it = std::find(_observers.begin(), _observers.end(), &obs);
		if (it != _observers.end())
			_observers.erase(it);
	};

	void Notify(Args... args)
	{
		for (auto it : _observers)
		{
			it->Update(args...);
		}
	};

private:
	std::vector<Observer<Args...>*> _observers;
};