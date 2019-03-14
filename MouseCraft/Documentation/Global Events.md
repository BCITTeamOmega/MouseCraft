# Global Events 
Intent: Notify *everyone* when an event occurs along with specific information. 

How: Use the **EventManager**, see syntax below. 

## Usage
```c++
// define data
struct Disaster
{
    int meteors = 500;
}

// create listener 
class Heretic : public ISubscriber
{
public:
    // Listen for the event
    Heretic() 
    {
        EventManager::Subscribe(EventName::APOCALYPSE, this);
    }
    ~Heretic()
    {
        EventManager::Unsubscribe(EventName::APOCALYPSE, this);
    }
    
    // Raise the event
    void TheEndIsHere()
    {
        EventManager::Notify(EventName::APOCALYPSE, new TypeParam<Disaster>(Diaster()));
        // can also do primitive types like: new TypeParam<float>(420);
    }
    
    // Implement the interface
    void Notify(EventName eventName, Param* params) override 
    {
    	if (eventName == EventName::APOCALYPSE)
    	{
    	    // ===> OVERHERE, THIS THICC BOY IS PROBABLY WHAT YOU'RE LOOKING FOR <===
            auto data = static_cast<TypeParam<Disaster>*>(params)->Param;
    	    
    	    if (data.meteors > 100)
      	        std::cout << "World is gone" << std::endl;
    	    else 
    	        std::cout << "Fallout 76" << std::endl;
    	}
    }
}
```