# Observer & Subject 
Intent: Define a one-to-many dependency between objects so that when *one* object changes state, *all* its dependents are notified and updated automatically. 

How: Use the **Subject, Observer, and/or Handler** to achieve this. 

## Usage
Below goes over two ways to implement this.
1. Subject & Handlers (Preferred)
2. Extending Subject & Observer

## Subject & Handler (Preferred)
### Subject
A subject notifies all attached observers with a given function signature.

Lets pretend you have a health object (wow we do!) that raises events on damage, heal, and death. 
```c++
#include "Event/Subject.h"
class Health
{
public:
	// Events 
	Subject<int> OnDamaged; 		// function signature eg. void Foobar(int value);
	Subject<int> OnHealed;
	Subject<> OnDeath;

	// Functions 
	void Damage(int amt)
	{
		hp -= amt;
		OnDamaged.Notify(amt);		// notify observers

		if (hp <= 0)
			OnDeath.Notify();	// notify observers
	}

	void Heal(int amt)
	{
		hp += amt;
		OnHealed.Notify(amt);		// notify observers
	}

private:
	// Variables
	int hp = 100;
};
```

### Handler
A Handler is a special Observer that points to a member function to be called when notified. You can have multiple handlers in one class.

Lets create a Dudette that plays SFX depending on the health event. 
```c++
#include "Event/Handler.h"
class Dudette
{
private:
	// Observers
	Handler<Dudette, int> handleOnDamaged;
	Handler<Dudette, int> handleOnHealed;
	Handler<Dudette>      handleOnDeath;

	// Functions
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
	
public:
	Dudette(Health& health) : 
		handleOnDamaged(this, &Dudette::PlayOuchSfx),   // point to your desired function
		handleOnHealed(this, &Dudette::ThankYou),
		handleOnDeath(this, &Dudette::Sayonara)
	{
		handleOnDamaged.Observe(health.OnDamaged);      // observe the subject 
		handleOnHealed.Observe(health.OnHealed);
		handleOnDeath.Observe(health.OnDeath);
		// btw you don't need to listen in the ctor, you could do it whenever
	}

	// no need to detach from dtor, automatic!
};
```
Using it is exactly the same!
```c++
// Create subject and observers
Health health;
Dudette* siameseTwinA  = new Dudette(health);
Dudette* siameseTwinB  = new Dudette(health); // observing the same health

health.Damage(50);                            // x2 "Ouchy"
health.Damage(50);                            // x2 "Ouchy", "Sayonara"
health.Heal(50);                              // x2 "Mucho Gracias"

// Subjects are unique
Health lonelyHealth;
lonelyHealth.Damage(50);                      // nothing happens - no one is listening to this poor guy :(

// (Extra) Manually attaching an observer
lonelyHealth.OnDamaged.Attach(siameseTwinA);  // now someone is listening!
lonelyHealth.Damage(50);                      // "Ouchy"
```

## Extending Subject and Observer
You can directly extend Subject to notify the observers manually. 

Below is a simple pair class that raises an event whenever one of it's value changes.
```c++
#include "Event/Subject.h"

// Inherit from Subject
class IntPair : public Subject<int,int>
{
public:
	void SetFirst(int amt)
	{
		first = amt;
		Notify(first, second);
	}

	void SetSecond(int amt)
	{
		second = amt;
		Notify(first, second);
		OnHealed.Notify(amt);   // notify observers
	}

	void Set(int left, int right)
	{
		first = left;
		second = right;
		Notify(first, second);
	}

private:
	int first;
	int second;
};
```

You can also directly extend Observer, simple solution if you are just listening for one Subject. 

```c++
#include "Event/Subject.h"
#include "Event/Observer.h"
// Inherit from Observer
class SumPrinter : public Observer<int, int>
{
public:
	void Notify(int first, int second) override
	{
		std::cout << (first + second) << std::endl;
	}
}
```

Observe by calling attach and detach

```c++
// Create Subject & Observer
IntPair pair;
SumPrinter printer; 

// Observe
pair.Attach(printer);
pair.SetFirst(400);    // "400"
pair.SetSecond(20);    // "420"

// Remember to manually detach 
pair.Detach(printer);
pair.Set(10, 20);      // nothing happens
```

## Attaching vs Observing
1. Use Attach when the **observer outlives the handler**. (eg. Health is destroyed before Dude)

This is because the observer has no reference or notion of the subject (safe to delete).

```c++
Subject<Arguments>.Attach(Handler<Arguments>);	
```

2. Use Observe when the **subject outlives the handler.** (eg. Dudette is destroyed before Health) 

This is because the handler automatically detaches on destruction (nullptr if subject is destroyed before handler).
```c++
Handler<Arguments>.Observe(Subject<Arguments>)
```

*If both are destroyed at the same time then whatever :^)*