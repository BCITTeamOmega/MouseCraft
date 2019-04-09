#pragma once
#include "Core\Component.h"
#include "Core\UpdatableComponent.h"
#include "Core\Entity.h"
#include "Event\ISubscriber.h"
#include "Physics/PhysicsComponent.h"
#include "Sound\SoundComponent.h"
#include "Event/Observer.h"
#include "PlayerComponent.h"
#include "Event/Handler.h"
#include "Physics/PhysObjectType.h"

constexpr auto CAT_JUMP_DIST = 7.5;

class Cat : public UpdatableComponent, public Observer<PhysicsComponent*>
{
public:
    Cat();
    ~Cat();
	void OnInitialized() override;
    void Update(float dt) override;
    void Notify(EventName eventName, Param* param) override;
	virtual void OnCollision(PhysicsComponent* e);
	virtual void OnHit(PhysicsComponent* e);

	Handler<Cat, PhysicsComponent*> HandleOnCollide;
	Handler<Cat, PhysicsComponent*> HandleOnHit;
    Entity* Hitbox;
private:
    // player id for checking input events
    int playerID;

    //time tracking variable
    float current_time;

    //update bools that let us know if we're in the middle of an animation
    bool isAttacking = false;
    bool isJumping = false;
    bool isPouncing = false;
    
    //method that launches attack animation and effects, and function that tracks if attack cooldown has ended
    void Attack();
    //helper function that handles the actual checks for hitting things
    void CheckHitbox(PhysicsComponent* pComp);
    void UpdateAttack(float dt);
    
    //method that is intended to remove player control and launch them like a projectile. A hit box will appear partway through.
    void Pounce(PhysicsComponent* pComp);

    //method that launches the attack/Pounce animation and effects, and the functions which track the cooldowns on both
    void Jump();
    void updatePounce(float dt);
};

