#pragma once

#include <iostream>
#include "Event/Subject.h"
#include "Event/Observer.h"
#include "Event/Handler.h"

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

// ===== HANDLER TEST ===== //
class FoobarRandom
{
public:
	FoobarRandom(Subject<int>& thermostat)
		: HandleTemperatureChange(this, &FoobarRandom::MyFunc)
	{
		HandleTemperatureChange.Observe(thermostat);
		// thermostat.Attach(&HandleTemperatureChange);
	};

	Handler<FoobarRandom, int> HandleTemperatureChange;

	void MyFunc(int temperature)
	{
		std::cout << temperature << std::endl;
	}
};

class Health
{
public:
	// Events 
	Subject<int> OnDamaged;
	Subject<int> OnHealed;
	Subject<> OnDeath;

	// Functions 
	void Damage(int amt)
	{
		hp -= amt;
		OnDamaged.Notify(amt);

		if (hp <= 0)
			OnDeath.Notify();
	}

	void Heal(int amt)
	{
		hp += amt;
		OnHealed.Notify(amt);
	}

private:
	// Variables
	int hp = 100;
};

class Dude : 
	public Observer<int>,
	public Observer<>
{
public:
	Dude(Health& health) : health(health)
	{
		health.OnDamaged.Attach(this);
		health.OnDeath.Attach(this);
	}

	~Dude()
	{
		health.OnDamaged.Detach(this);
		health.OnDeath.Detach(this);
	}

	// OnDamaged
	void Publish(int damage) override
	{
		std::cout << "PLAY_SFX: minecraft_oof.wav" << std::endl;
	}

	// OnDeath
	void Publish() override
	{
		std::cout << "W A S T E D" << std::endl;
	}

private:
	Health& health;
};

class Dudette
{
public:
	Dudette(Health& health) : 
		handleOnDamaged(this, &Dudette::PlayOuchSfx),	// point to your desired function
		handleOnHealed(this, &Dudette::ThankYou),
		handleOnDeath(this, &Dudette::Sayonara)
	{
		handleOnDamaged.Observe(health.OnDamaged);		// listen to the event
		handleOnHealed.Observe(health.OnHealed);
		handleOnDeath.Observe(health.OnDeath);
	}

	// no need to detach from dtor!

private:
	Handler<Dudette, int> handleOnDamaged;
	Handler<Dudette, int> handleOnHealed;
	Handler<Dudette>	  handleOnDeath;

	void PlayOuchSfx(int damage)
	{
		std::cout << "Ouchy" << std::endl;
	}

	void ThankYou(int amount)
	{
		std::cout << "Mucho Gracias" << std::endl;
	}

	void Sayonara()
	{
		std::cout << "Sayonara" << std::endl;
	}
};