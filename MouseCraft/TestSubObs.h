#pragma once

#include <iostream>
#include "Subject.h"
#include "Observer.h"

class FreezingObserver : public Observer<int>
{
public:
	void Publish(int i) override
	{
		if (i <= 0)
		{
			std::cout << "FREEZING!" << std::endl;
		}
		else
		{
			std::cout << "FreezingObserver observed " << i << std::endl;
		}
	}
};

class BoilingObserver : public Observer<int>
{
public:
	void Publish(int i) override
	{
		if (i >= 100)
		{
			std::cout << "BOILING!" << std::endl;
		}
		else
		{
			std::cout << "BoilingObserver observed " << i << std::endl;
		}
	}
};

// Extending a subject
class FoobarSubject : public Subject<float, int>
{
public:
	FoobarSubject() {}
	~FoobarSubject() {}
	
	void Set(float f, int i)
	{
		fVal = f;
		iVal = i;
		Notify(fVal, iVal);
	}

	void Set(float f)
	{
		fVal = f;
		Notify(fVal, iVal);
	}

	void Set(int i)
	{
		iVal = i;
		Notify(fVal, iVal);
	}

private: 
	float fVal;
	int iVal;
};

class FoobarObserver : public Observer<float, int>
{
public:
	void Publish(float f, int i) override
	{
		std::cout << "Float: " << f << " Int: " << i << std::endl;
	}
};