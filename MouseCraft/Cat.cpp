#include "Cat.h"
#include "Event\EventManager.h"
#include "Input\InputSystem.h"
#include "Obstacle.h"
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

	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();
	pComp->updateFalling();

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

void Cat::OnHit(PhysicsComponent* other)
{

}

//check if we are doing something, then attack
void Cat::Attack()
{
    //get our own physics component
    PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();
    
    //block if we are in an animation already
    if (isAttacking || pComp->isJumping || isPouncing) {
        return;
    }

    //check which level we're on
    std::set<PhysObjectType::PhysObjectType> targets;
    if (pComp->isUp) {
        //generate check type
        targets = std::set<PhysObjectType::PhysObjectType>{
            PhysObjectType::OBSTACLE_UP,
            PhysObjectType::MOUSE_UP
        };
    } else {
        //generate check type
        targets = std::set<PhysObjectType::PhysObjectType>{
            PhysObjectType::OBSTACLE_DOWN,
            PhysObjectType::MOUSE_DOWN
        };
    }

    //determine our position for area check
    //get the section directly in front of our world position
    auto p1 = GetEntity()->transform;
    auto pos = p1.getWorldPosition();
    pos += p1.getWorldForward();
    //get the corners for our bounding box
    auto bl = pos + glm::vec3(-10, 0, -10);
    auto tr = pos + glm::vec3(10, 0, 10);

    //launch area check
    auto results = pComp->areaCheck(targets, new Vector2D(bl.x, bl.z), new Vector2D(tr.x, tr.z));

	//calculate which direction we're attacking (locked to the 4 cardinal directions)
	auto angle = GetEntity()->transform.getWorldRotation2D();
	
	Vector2D facing;
	if (angle > -M_PI/4 && angle < M_PI/4)			// up
		facing = Vector2D(0, -1);
	else if (angle > M_PI/4 && angle < M_PI*3/4)	// left
		facing = Vector2D(-1, 0);	
	else if (angle > -M_PI*3/4 && angle < -M_PI/4)	// right 
		facing = Vector2D(1, 0);
	else 											// down 
		facing = Vector2D(0, 1);

	std::cout << angle << std::endl;
	std::cout << facing.x << "," << facing.y << std::endl;

    //check if we hit something
    if (results.size() > 0) {

		for (auto& p : results)
		{
			if (p->type == PhysObjectType::MOUSE_UP || p->type == PhysObjectType::MOUSE_DOWN)
			{
				p->GetEntity()->GetComponent<HealthComponent>()->Damage(1);
			}
			else
			{
				auto e = p->GetEntity();
				p->GetEntity()->GetComponent<Obstacle>()->HitByCat(facing);
			}
		}
    }

	//MIGHT BE COOL TO PLAY A WHACK SOUND EFFECT IF SOMETHING IS HIT

    GetEntity()->GetComponent<SoundComponent>()->ChangeSound(SoundsList::Swipe); //set sound to swipe
    auto ourPos = GetEntity()->transform.getLocalPosition(); //get our current position
    GetEntity()->GetComponent<SoundComponent>()->PlaySound(ourPos.x, ourPos.y, ourPos.z); //play sound

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
void Cat::Jump()
{
	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();

	if (pComp != nullptr)
	{
		//block if we are in an animation already
		if (isAttacking || pComp->isJumping || isPouncing)
			return;

		//position of cat
		Vector2D* curPos = new Vector2D(GetEntity()->transform.getLocalPosition().x, GetEntity()->transform.getLocalPosition().z);
		//vector in front of cat of length = JUMP_DIST
		Vector2D* jumpVec = new Vector2D(GetEntity()->transform.getLocalForward().x * CAT_JUMP_DIST, GetEntity()->transform.getLocalForward().z * CAT_JUMP_DIST);
		jumpVec = new Vector2D(*curPos + *jumpVec);

		std::set<PhysObjectType::PhysObjectType> types = std::set<PhysObjectType::PhysObjectType>{
			PhysObjectType::PLATFORM
		};

		Vector2D* hitPos = new Vector2D(0, 0);

		PhysicsComponent* jumpTarget =  pComp->rayCheck(types, curPos, jumpVec, *hitPos);

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
	}

    //pounce code
    std::cout << "Cat has pounced." << std::endl;
    //isPouncing = true;
}

// track the time since we launched the jump animation, and reset when finished
void Cat::UpdateJump(float dt)
{
    /*if (current_time < JUMP_TIME) {
        //advance time
        current_time += dt;
        return;
    }
    isJumping = false;
    current_time = 0;*/
}

// track the time since we launched the pounce animation, and reset when finished
void Cat::updatePounce(float dt)
{
    /*if (current_time < POUNCE_TIME) {
        //advance time
        current_time += dt;
        return;
    }
    isPouncing = false;
    current_time = 0;*/
}
