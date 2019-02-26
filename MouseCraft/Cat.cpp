#include "Cat.h"
#include "Event\EventManager.h"
#include "Input\InputSystem.h"
#include <iostream>

#define ATTACK_TIME 5
#define JUMP_TIME 5
#define POUNCE_TIME 5

Cat::Cat() {
    EventManager::Subscribe(INPUT_BUTTON, this);
    playerID = 0;
    current_time = 0;
}


Cat::~Cat()
{
}

void Cat::Update(float dt) {
    if (isAttacking) {
        UpdateAttack(dt);
    }
    if (isJumping) {
        UpdateJump(dt);
    }
    if (isPouncing) {
        updatePounce(dt);
    }
}

void Cat::OnInitialized()
{
	//Listens for collisions with the physics component
	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();

	if (pComp != nullptr)
		pComp->onCollide.Attach(this);
}

//check for button presses and then call functions
void Cat::Notify(EventName eventName, Param * param) {
    UpdatableComponent::Notify(eventName, param);
    if (eventName == INPUT_BUTTON) {
        auto data = static_cast<TypeParam<ButtonEvent>*>(param)->Param;
        if (data.player != playerID || data.isDown != true) {
            return;
        }

        switch (data.button)
        {
        case Button::PRIMARY:
        case Button::SECONDARY:
            Attack();
            break;
        case Button::AUX1:
            Jump();
            break;
        default:
            break;
        }
    }
}

//this function will call the physics/map system to determine if we're at a jump location (floor -> platform)
bool Cat::canJump()
{
    //something something colt physics stuff

    //temp
    int i = rand() % 2;
    
    return i == 1 ? true : false;
}

//check if we are doing something, then attack
void Cat::Attack()
{
    //block if we are in an animation already
    if (isAttacking || isJumping || isPouncing) {
        return;
    }
    //actually launch attack
    std::cout << "Cat has attacked." << std::endl;
    isAttacking = true;
}

// track the time since we launched the attack animation, and reset when finished
void Cat::UpdateAttack(float dt) {
    if (current_time < ATTACK_TIME) {
        //advance time
        current_time += dt;
        return;
    }
    isAttacking = false;
    current_time = 0;
}

//check if we are doing something, then check if we can jump and either jump or pounce
void Cat::Jump() {
    //block if we are in an animation already
    if (isAttacking || isJumping || isPouncing) {
        return;
    }
    //check if we are in a location we can jump in
    if (canJump()) {
        //Jump code
        std::cout << "Cat has jumped." << std::endl;
        isJumping = true;
        return;
    }
    //pounce code
    std::cout << "Cat has pounced." << std::endl;
    isPouncing = true;
}

// track the time since we launched the jump animation, and reset when finished
void Cat::UpdateJump(float dt)
{
    
    if (current_time < JUMP_TIME) {
        //advance time
        current_time += dt;
        return;
    }
    isJumping = false;
    current_time = 0;
}

// track the time since we launched the pounce animation, and reset when finished
void Cat::updatePounce(float dt)
{
    if (current_time < POUNCE_TIME) {
        //advance time
        current_time += dt;
        return;
    }
    isPouncing = false;
    current_time = 0;
}
