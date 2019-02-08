#pragma once

#include <typeindex>
#include <typeinfo>

class Entity;

class Component
{
public:
	Component();
	virtual ~Component();
	virtual Component& operator=(const Component&) = delete;  // Disallow copying
	Component(const Component&) = delete;

	// WARNING: Should only be called internally by the engine. 
	// Initializes this component.
	void Initialize();

	// Returns if this component is initialized already. 
	bool GetInitialized() const { return _initialized; }
	
	// Called when this component is initialized (added to the active scene).
	// Only ever called once. Overridable.
	virtual void OnInitialized() {};

	// Sets the enable status of this component.
	void SetEnabled(bool enabled) { _enabled = enabled; }

	// Returns if this component is enabled 
	bool GetEnabled() const { return _enabled; }
	
	// Returns the entity this component belongs to. 
	Entity* GetEntity() const { return _entity; }

	// Sets the entity this component belongs to.
	void SetEntity(Entity* e) { _entity = e; }	

	// Returns the ID of this component. Guaranteed to be unique from all components.
	unsigned int GetID() const { return _id; }

	// Returns true if this component is active (must be enabled, must have entity, entity must be active).
	bool GetActive() const;

private:
	bool _initialized = false;
	bool _enabled = true;
	Entity* _entity;
	static unsigned int _curID;
	unsigned int _id;
};