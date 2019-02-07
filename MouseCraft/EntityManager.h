#pragma once

#include <vector>
#include "Core/Entity.h"
#include "EventManager/EventManager.h"

// Convenience class to retrieve all entities (including ones 
// not in the scene root) in a flat vector. 
class EntityManager : public ISubscriber
{
public:
// singleton 
public:
	static EntityManager& instance()
	{
		static EntityManager _instance;
		return _instance;
	}
	EntityManager(EntityManager const&) = delete;
	void operator=(EntityManager const&) = delete;
private:
	EntityManager() 
	{
		EventManager::Subscribe(EventName::ENTITY_CREATED, this);
		EventManager::Subscribe(EventName::ENTITY_DESTROYED, this);
	};
	~EntityManager() {};

// variables 
private:
	std::vector<Entity*> entities;

// functions 
public:
	const std::vector<Entity*>& GetEntities()
	{
		return entities;
	}

	virtual void Notify(EventName eventName, Param *params)
	{
		auto entityParam = static_cast<TypeParam<Entity*>*>(params);
		if (eventName == EventName::ENTITY_CREATED)
		{
			entities.push_back(entityParam->Param);
		}
		else if (eventName == EventName::ENTITY_DESTROYED)
		{
			entities.erase(
				std::remove(entities.begin(), entities.end(), entityParam->Param), 
				entities.end());
		}
	}
};

/*
Notes: 
This class does not manage the lifecycle of the entity in any way 
(may change in the future). This is b/c the Entity already has strong 
intentional coupling with the Engine (requires knowledge of active scene 
for active status and engnie for deferred exeuction handling). 

Considering changing this in the future to aid cohesion but this 
would increase coupling from 2 (Entity+Engine) 
to 3 (Engine + EntityManager + SceneManager? + Entity? if abstracting Entity.Destroy() etc.) 
*/