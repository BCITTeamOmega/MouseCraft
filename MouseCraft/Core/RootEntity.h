#pragma once

#include "Entity.h"

#include "../Scene.h"

// Special entity for scene root. 
class RootEntity : public Entity
{
public:
	RootEntity(Scene* scene) : Entity(0u)
	{
		_myScene = scene;
		setEnabled(false);
	}

	template<class T>
	void addComponent() = delete;

	void setParent(Entity* parent) = delete;

	void sceneActivated()
	{
		setEnabled(true, true);
	}

	void sceneDeactivated()
	{
		setEnabled(false, true);
	}
};