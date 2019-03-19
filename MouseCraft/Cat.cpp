#include "Cat.h"
#include "Event\EventManager.h"
#include "Input\InputSystem.h"
#include <iostream>

#define ATTACK_TIME 5
#define JUMP_TIME 5
#define POUNCE_TIME 5

Cat::Cat() :
	HandleOnCollide(this, &Cat::OnCollision),
	HandleOnHit(this, &Cat::OnHit)
{
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
	{
		HandleOnCollide.Observe(pComp->onCollide);
		HandleOnHit.Observe(pComp->onHit);
	}

    playerID = GetEntity()->GetComponent<PlayerComponent>()->GetID();
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

void Cat::OnCollision(PhysicsComponent * pc)
{
	
}

void Cat::OnHit(PhysicsComponent* e)
{

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

	PhysicsComponent* jumpTarget = canJump();

	//check if we are in a location we can jump in
	if (jumpTarget != nullptr) {
		//Jump code
		std::cout << "Cat has jumped." << std::endl;
		GetEntity()->GetComponent<PhysicsComponent>()->isJumping = true;
		isJumping = true;

		GetEntity()->GetComponent<SoundComponent>()->ChangeSound(SoundsList::Jump); //set sound to jump
		auto pos = GetEntity()->transform.getLocalPosition(); //get our current position
		GetEntity()->GetComponent<SoundComponent>()->PlaySound(pos.x, pos.y, pos.z); //play sound
		return;
	}

    //pounce code
    std::cout << "Cat has pounced." << std::endl;
    isPouncing = true;
}

PhysicsComponent* Cat::canJump()
{
	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();

	if (pComp != nullptr)
	{
		Vector2D* curPos = new Vector2D(GetEntity()->transform.getLocalPosition().x, GetEntity()->transform.getLocalPosition().z);
		Vector2D* forward = new Vector2D(GetEntity()->transform.getLocalForward().x * JUMP_DIST, GetEntity()->transform.getLocalForward().z * JUMP_DIST);
		Vector2D* target = new Vector2D(*curPos + *forward);
		Vector2D* hitPos;

		std::vector<PhysObjectType::PhysObjectType> types;
		types.push_back(PhysObjectType::PLATFORM);

		PhysicsComponent* hit = pComp->rayCheck(types, curPos, target, *hitPos);
	}

	return nullptr;
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
