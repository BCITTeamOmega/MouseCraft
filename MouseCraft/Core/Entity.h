#pragma once

#define GLM_ENABLE_EXPERIMENTAL	// I have no idea why we can't put this in main

#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <typeinfo>
#include <typeindex>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Component.h"
#include "Transform.h"

class Entity
{
// Type definitions 
	typedef std::unordered_map <std::type_index, std::unique_ptr<Component>> ComponentMap;

// Variables 
public:
	Transform transform;

private:
	static unsigned int _curID;
	unsigned int _id = 0;
	bool _enabled = true;
	bool _static = false;
	ComponentMap _components;					// component map (deprecated)
	std::vector<Component*> _componentStorage;	// component storage
	std::vector<Entity*> _children;
	Entity* _parent;

// Functions 
public: 
	Entity();
	Entity(Entity* parent);	
	Entity(unsigned int id);	// don't call this unless you know what you're doing.
	~Entity();

	// Returns entity's ID. This cannot change. 
	unsigned int getID() const;

	// TODO: Optimize
	// Returns entity's active status. 
	// Only true if in active scene, all parents enabled, and this is enabled.
	bool getActive() const;

	// Returns entity's enabled status. Does not check if parent is enabled.
	bool getEnabled() const;

	// Set entity's enabled status. Does not change children's status. 
	bool setEnabled(bool enabled);
	
	// gets the entity's static status. 
	bool getStatic() const;

	// Sets the static flag on this entity. Setting this to true will completely freeze all transforms 
	// on this entity and it's children. Moving the parent (even if not static) will not move this entity.
	// ONLY call when you're done configuring it.
	void setStatic(bool torf);

	// Set the parent for this entity. Will properly bind entities together. 
	void setParent(Entity* parent);

	// Returns the parent entity. Can be null.
	Entity* getParent() const;
	
	// Adds entity as a child. Will properly bind entities together.
	void addChild(Entity* child);

	// Returns a reference to child entities. 
	std::vector<Entity*> const& getChildren() const;

	// template definitions "must" be in header
	// https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl

	// DEPRECATED. Creates and adds a component to this entity. Component will be bound to this entity. 
	template<class T>
	void addComponent() {
		_components[typeid(T)] = std::make_unique<T>();
		_components[typeid(T)]->setEntity(this);
		_componentStorage.push_back(_components[typeid(T)].get());
	}

	// Adds a component to this entity.
	void addComponent(Component* component);

	// Returns a pointer to specified component. 
	template<class T>
	T* getComponent()
	{
		return static_cast<T*>(_components[typeid(T)].get());
	}

	// Returns a copy of all components attached to this entity. 
	const std::vector<Component*>& getComponents()
	{
		return _componentStorage;
	}

	// Remove and destroy a component attached to this entity. 
	template<class T>
	void removeComponent()
	{
		if (getComponent<T>() != nullptr)
		{
			getComponent<T>()->Kill();
			_componentStorage.erase(_components[typeid(T)].get());
			_components.erase(typeid(T));
		}
	}

	// Inform this entity to destroy itself. 
	void destroy();

	// Release resources. Only call it if you REALLY know what you're doing.
	// Call destroy() otherwise.
	void release();

private:
	// Helper method to remove child with ID. This should only be called internally, as bindings will not be correct.
	void removeChild(unsigned int id);

	// Helper method to properly bind a parent and child entity together. 
	static void bindEntities(Entity* parent, Entity* child);

};