#pragma once
#include "Core\Component.h"
#include "Core\UpdatableComponent.h"
#include "Core\Entity.h"
#include "Event\ISubscriber.h"


class Cat : public UpdatableComponent
{
public:
    Cat();
    ~Cat();
    void Update(float dt) override;
    void Notify(EventName eventName, Param* param) override;
    void setPlayer(int ID) { playerID = ID; }
private:
    // player id for checking input events
    int playerID;

    //time tracking variable
    float current_time;

    //update bools that let us know if we're in the middle of an animaktion
    bool isAttacking = false;
    bool isJumping = false;
    bool isPouncing = false;
    
    //bool function that let us know if we are in a zone where jumping is possible (Bother colt about this later)
    bool canJump();
    
    //method that launches attack animation and effects, and function that tracks if attack cooldown has ended
    void Attack();
    void UpdateAttack(float dt);
    
    //method that launches the Jump/Pounce animation and effects, and the functions which track the cooldowns on both
    void Jump();
    void UpdateJump(float dt);
    void updatePounce(float dt);
};

