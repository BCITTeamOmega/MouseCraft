#include "Component.h"
#include <iostream>
#include "../EventManager/EventManager.h"
#include "ComponentManager.h"

unsigned int Component::_curID = 0;

Component::Component() : _id(Component::_curID++)
{
	std::cout << "Component created" << std::endl;
	//TypeParam<Component*> param(this);
	//EventManager::Notify(COMPONENT_ADDED, &param, false);
}

Component::~Component()
{
	std::cout << "Component destroyed" << std::endl;
	TypeParam<Component*> param(this);
	EventManager::Notify(COMPONENT_REMOVED, &param, false);
}

// Note: Looks kind of strange but this is to prevent components
// from being initialized twice and hides this interaction 
// away from implementing classes.
// Note: Implementors should extend onInitialized() 
// Consider: Deleting this, as it's currently handled by Entity. 
void Component::initialize()
{
	if (_initialized)
	{
		std::cerr << "WARNING: Component tried to init twice - how did that happen?" << std::endl;
		return;
	}
	_initialized = true;
	onInitialized();
}
