#pragma once

#include "Entity.h"

#include "Scene.h"

// Special entity for scene root. 
class RootEntity : public Entity
{
public:
	RootEntity(Scene* scene) : Entity(0u)
	{
		_myScene = scene;
	}

	template<class T>
	void AddComponent() = delete;

	void SetParent(Entity* parent) = delete;

	void SceneActivated()
	{
		SetEnabled(true, true);
	}

	void SceneDeactivated()
	{
		SetEnabled(false, true);
	}
};