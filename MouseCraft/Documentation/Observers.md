# Observer & Subject 
Intent: Define a one-to-many dependency between objects so that when *one* object changes state, *all* its dependents are notified and updated automatically. 
How: Use the **Subject, Observer, and/or Handler** to achieve this. 

## Usage
Below goes over two ways to implement this, read both as the 2nd approach builds on the 1st one.
1. Subject & Extending Observer
2. Subject & Handlers (Preferred)

## Subect & Extending Observer
Lets pretend you have a health object (wow we do!) 
```c++
#include "Event/Subject.h"
class Health
{
public:
	// Events 
	Subject<int> OnDamaged; // function signature eg. void Foobar(int value);
	Subject<int> OnHealed;
	Subject<> OnDeath;

	// Functions 
	void Damage(int amt)
	{
		hp -= amt;
		OnDamaged.Notify(amt);  // notify observers

		if (hp <= 0)
			OnDeath.Notify();   // notify observers
	}

	void Heal(int amt)
	{
		hp += amt;
		OnHealed.Notify(amt);   // notify observers
	}

private:
	// Variables
	int hp = 100;
};
```
And we have Dude that handles SFX whenever we're hit.
```c++
#include "Event/Subject.h"
#include "Event/Observer.h"
class Dude : 
	public Observer<int>,   // listening for this function signature
	public Observer<>       // ... and this one too.
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
```
Lets give it a try!
```c++
// Create Subjects and Observers
Health health;
Dude* siameseTwinA  = new Dude(health);
Dude* siameseTwinB  = new Dude(health); // sharing (observing) the same health

health.Damage(50);	// x2 "PLAY_SFX: minecraft_oof.wav"
health.Heal(50);	// nothing happens 
health.Damage(100);	// x2 "PLAY_SFX: minecraft_oof.wav", "W A S T E D"

// Subjects are unique
Health lonelyHealth;
lonelyHealth.Damage(50);    // happens - no one is listening to this poor guy :(

lonelyHealth.OnDamaged.Attach(siameseTwinA);    // now someone is listening!
lonelyHealth.Damage(50);    // "PLAY_SFX: minecraft_oof.wav"
```
## Subject & Handlers
The syntax above wasn't the prettiest and you may have noticed that we can't listen for the OnHealed event too! 
Instead, **the preferred way to observe a subject is with the Handler** - a special observer. 
A Handler points to a member function to be called when notified.

This time we have a Dudette that wants to listen for all three health events. 
```c++
#include "Event/Handler.h"
class Dudette
{
private:
    // Observers
	Handler<Dudette, int> handleOnDamaged;
	Handler<Dudette, int> handleOnHealed;
	Handler<Dudette>	  handleOnDeath;

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
		handleOnDamaged(this, &Dudette::PlayOuchSfx),	// point to your desired function
		handleOnHealed(this, &Dudette::ThankYou),
		handleOnDeath(this, &Dudette::Sayonara)
	{
		handleOnDamaged.Observe(health.OnDamaged);		// listen to the event (use the Observe() function too!)
		handleOnHealed.Observe(health.OnHealed);
		handleOnDeath.Observe(health.OnDeath);
		// btw you don't need to listen in the ctor, you could do it whenever
	}

	// no need to detach from dtor, automatic!
};
```
Using it is exactly the same!
```c++
Dudette* dudette = new Dudette(health);
health.Heal(100);	// "thanks"
health.Damage(50);	// "ouch"
health.Damage(50);	// "sayonara"
delete(dudette);    // stop listening
```
