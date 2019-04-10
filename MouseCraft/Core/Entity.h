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

class Scene;

class Entity
{
// Variables 
public:
	Transform transform;
	std::string name;

protected:
	Scene* _myScene;	// which scene this entity is in. null if not assigned.
	
private:
	static unsigned int _curID;
	unsigned int _id = 0;
	bool _enabled = true;
	bool _static = false;
	bool _initialized = false;
	std::vector<Component*> _components;	// component storage
	std::vector<Entity*> _children;
	Entity* _parent;

// Functions 
public: 
	Entity();
	Entity(unsigned int id);	// WARNING: don't call this unless you know what you're doing.
	~Entity();

	// Returns entity's ID. This cannot change. 
	unsigned int GetID() const;

	// WARNING: Should only be called internally by the engine.
	// Called when added into the active scene. 
	void Initialize();

	// TODO: Optimize
	// Returns entity's active status. 
	// Only true if in active scene, all parents enabled, and this is enabled.
	bool GetActive() const;

	// Returns entity's enabled status. Does not check if parent is enabled.
	bool GetEnabled() const;

	// Set entity's enabled status. Does not change children's status. 
	void SetEnabled(bool enabled, bool force = false);	
	
	// gets the entity's static status. 
	bool GetStatic() const;

	// Sets the static flag on this entity. Setting this to true will completely freeze all transforms 
	// on this entity and it's children. Moving the parent (even if not static) will not move this entity.
	// ONLY call when you're done configuring it.
	void SetStatic(bool torf);

	// Alias to get transform
	inline Transform& t() { return transform; } 

	// Returns the parent entity. Can be null.
	Entity* GetParent() const;

	// Set the parent for this entity. Will properly bind entities together. 
	void SetParent(Entity* parent, bool force = false);

	// Adds entity as a child. Will properly bind entities together.
	void AddChild(Entity* child, bool force = false);

	// Returns a reference to child entities. 
	std::vector<Entity*> const& GetChildren() const;

	// Adds a component to this entity.
	void AddComponent(Component* component);

	// Returns a pointer to specified component.
	// Note: This is a relatively expensive operation, cache your results. 
	template<class T>
	T* GetComponent()
	{
		// Rationale: This function should not be called often.
		// If you need to constantly call this cache it or consider using a system.
		for (const auto& c : _components)
		{
			auto found = dynamic_cast<T*>(c);
			if (found) return found;
		}
		return nullptr;
	}

	// Returns all components attached to this entity. 
	const std::vector<Component*>& GetComponents()
	{
		return _components;
	}

	// Removes and destroys the first instance of a component type attached to this entity. 
	// Does nothing if not found. 
	template<class T>
	void RemoveComponent()
	{
		for (auto it = _components.begin(); it != _components.end(); )
		{
			auto found = dynamic_cast<T*>(*it);
			if (found)
			{
				delete(*it);
				_components.erase(it);
				return;
			}
			else
			{
				++it;
			}
		}
	}

	// Removes and destroy a specific component.
	void RemoveComponent(Component* c);

	// Inform this entity to destroy itself. 
	void Destroy(bool force = false);

	// DEPRECATED
	// Release resources. Only call it if you REALLY know what you're doing.
	// Call destroy() otherwise.
	void Release();

protected:
	bool isInActiveScene() const;

	void setScene(Scene* scene);

	Scene* getScene() const;
	
private:
	// Helper method to remove child with ID. This should only be called internally, as bindings will not be correct.
	void removeChild(unsigned int id);

	// Helper method to properly bind a parent and child entity together. 
	// Any method that moves an entity around will always go through this function.
	static void bindEntities(Entity* parent, Entity* child);

	// Helper method to determine if all parents are enabled. 
	bool getParentEnabled() const;
};