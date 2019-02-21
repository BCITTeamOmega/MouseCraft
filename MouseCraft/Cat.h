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
private:
    int player;
    void Attack();
    void Jump();
};

