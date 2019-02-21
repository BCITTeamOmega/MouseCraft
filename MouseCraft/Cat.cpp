#include "Cat.h"
#include "Event\EventManager.h"
#include "Input\InputSystem.h"


Cat::Cat() {
    EventManager::Subscribe(INPUT_BUTTON, this);
}


Cat::~Cat()
{
}

void Cat::Update(float dt) {

}



void Cat::Notify(EventName eventName, Param * param) {
    UpdatableComponent::Notify(eventName, param);
    switch (eventName) {
    case INPUT_BUTTON:
        auto data = static_cast<TypeParam<ButtonEvent>*>(param)->Param;
        
        break;
    default:
        break;
    }
}
