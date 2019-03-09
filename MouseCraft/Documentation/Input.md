# Input 
Intent: Respond to player input

How: Listen for input events via the **EventManager**.

Notes: 
- Only joystick input is implemented
- Player ID 10 has debug keyboard input (emulates a controller)
- There is no mouse input at the moment :(

## Usage
```c++
class PlayerComponent : public ISubscriber
{
public:
    // Variables 
    ... 
    
    // Listen
    PlayerComponent()
    {
        EventManager::Subscribe(EventName::INPUT_AXIS_2D, this);
    }
    
    // Handle 
    void PlayerComponent::Notify(EventName eventName, Param* params)
    {
        if (eventName == EventName::INPUT_AXIS_2D)
        {
            auto data = static_cast<TypeParam<Axis2DEvent>*>(params)->Param;
            
            if (data.player != _playerID)
                return;
            
            if (data.axis == Axis::LEFT)
            {
                _move = data.GetClamped();
            }
            else if (data.axis == Axis::RIGHT)
            {
                _aim = data.GetClamped();
            }
        }
    }
}
```

## Enumerations
```c++
enum EventName {
    ...
    INPUT_RAW,      // SDL2 event (not-impelemnted)
    INPUT_AXIS,     // AxisEvent
    INPUT_AXIS_2D,  // Axis2DEvent
    INPUT_BUTTON,   // ButtonEvent
    ...
}

enum Axis
{
    LEFT_HOR,
    LEFT_VER,
    RIGHT_HOR,
    RIGHT_VER,
    LEFT,        
    RIGHT,        
};

enum Button
{
    PRIMARY,    // R1                
    SECONDARY,  // L1                
    AUX1,       // SOUTH BUTTON    
    AUX2,       // WEST BUTTON    
};
```

## Datatypes
```c++
struct AxisEvent
{
    int player;
    Axis axis;
    float value;
};

struct Axis2DEvent
{
    int player;
    Axis axis;
    glm::vec2 value;
    // returns value normalized (length of 1)
    glm::vec2 GetDir();
    // returns value with length clamped between 0-1
    glm::vec2 GetClamped();
};

struct ButtonEvent
{
    int player;
    Button button;
    bool isDown;
};
```

