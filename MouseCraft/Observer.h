#pragma once

/// <summary>
/// Implementation of the observer pattern.
/// Allows you to listen an "event" on a specific object.
/// </summary>
template<typename ... Args>
class Observer
{
public:
	Observer() {};
	~Observer() {};
	virtual void Update(Args... args) {}
};