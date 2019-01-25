#pragma once

#include "Entity.h"

// Special entity for scene root. 
class RootEntity : public Entity
{
public:
	RootEntity() : Entity(0u)
	{
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