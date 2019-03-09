#include "Entity.h"
#include "OmegaEngine.h"
#include "../Event/EventManager.h"
#include <iostream>

unsigned int Entity::_curID = 0;

Entity::Entity() :_id(++Entity::_curID) 
{ 
	EventManager::Notify(EventName::ENTITY_CREATED, new TypeParam<Entity*>(this));
}

Entity::Entity(unsigned int id) : _id(id)
{
	// do not notify entity manager
	std::cout << "Entity created with custom ID: " << _id << std::endl;
}

Entity::~Entity()
{
	EventManager::Notify(EventName::ENTITY_DESTROYED, new TypeParam<Entity*>(this));
	std::cout << "Entity: " << _id << " destroyed" << std::endl;
}

unsigned int Entity::GetID() const
{
	return _id;
}

void Entity::Initialize()
{
	if (!_initialized)
	{
		std::cout << "Entity already initialized" << std::endl;
		
		for (auto& c : _components)
		{
			c->Initialize();
		}
		_initialized = true;
	}

	// (ensure) initialize children 
	for (auto& e : _children)
	{
		e->Initialize();
	}
}

bool Entity::GetActive() const
{
	return (_enabled && isInActiveScene() && getParentEnabled());
}

bool Entity::GetEnabled() const
{
	return _enabled;
}

void Entity::SetEnabled(bool enabled, bool force)
{
	// ensure actual change 
	if (_enabled == enabled) return;

	// explicit or implicit instant change  
	if (force || !isInActiveScene())
	{
		_enabled = enabled;
	}
	else // defer 
	{
		OmegaEngine::Instance().DeferAction(
			new StatusActionParam((enabled) ? StatusActionType::Enable : StatusActionType::Disable, this));
	}
}

bool Entity::GetStatic() const
{
	return _static;
}

void Entity::SetStatic(bool torf)
{
	// todo: implement properly
	if (torf)
	{
		transform.computeLocalTransformation();
		transform.computeWorldTransformation();

		_static = true;				// freeze data 
		for (auto& e : _children)	// apply to all children 
			e->SetStatic(true);
	}
	else
	{
		_static = false;
		for (auto& e : _children)
			e->SetStatic(false);
	}
}

void Entity::SetParent(Entity* parent, bool force)
{
	// ensure actual change
	if (parent == this) return;

	// explicit or implicit instant change  
	bool childOrParentInScene = isInActiveScene() || (parent != nullptr && parent->isInActiveScene());
	if (force || !childOrParentInScene)
	{
		Entity::bindEntities(parent, this);
	}
	else // defer 
	{
		OmegaEngine::Instance().DeferAction(
			new StatusActionParam(StatusActionType::Move, this, parent));
	}
}

Entity* Entity::GetParent() const
{
	return _parent;
}

// Warning: this function does not check if this child has already been added. 
void Entity::AddChild(Entity* child, bool force)
{
	if (child == nullptr)
	{
		std::cerr << "ERROR: Entity::addChild() adding null child, please check your code!" << std::endl;
		return;
	}

	// explicit or implicit instant change  
	bool childOrParentInScene = isInActiveScene() || child->isInActiveScene();
	if (force || !childOrParentInScene)
	{
		Entity::bindEntities(this, child);
	}
	else // defer 
	{
		OmegaEngine::Instance().DeferAction(
			new StatusActionParam(StatusActionType::Move, child, this));
	}
}

void Entity::removeChild(unsigned int id)
{
	auto t = std::find_if(_children.begin(), _children.end(), [&id](const Entity* e) { return e->GetID() == id; });
	_children.erase(t);
}

void Entity::bindEntities(Entity * parent, Entity * child)
{
	if (!child)
	{
		std::cerr << "ERROR: Entity::bindEntities() called with null child. This case should never occur." << std::endl;
		return;
	}
	
	if (child->_parent)
		child->_parent->removeChild(child->_id);

	if (parent)
	{
		parent->_children.push_back(child);
		child->_parent = parent;
		child->setScene(parent->getScene());

		// initialize? 
		if (parent->isInActiveScene())
		{
			child->Initialize();
		}
	}
	else
	{
		child->_parent = nullptr;
		child->setScene(nullptr);
	}
}

// Despite being recursive this performs very well 
// (comparable to local variable even in worst case scenario - all enabled)
bool Entity::getParentEnabled() const
{
	// fast stop 
	if (!_enabled)
	{
		return false;
	}

	if (_parent)
	{
		return _parent->getParentEnabled();
	}

	return true;
}

std::vector<Entity*> const& Entity::GetChildren() const
{
	return _children;
}

void Entity::AddComponent(Component * component)
{
	component->SetEntity(this);
	_components.push_back(component);
}

void Entity::RemoveComponent(Component * c)
{
	auto it = std::find(_components.begin(), _components.end(), c);
	if (it != _components.end())
	{
		delete(*it);
		_components.erase(it);
	}
	/* 
	_componentStorage.erase(
		std::remove(_componentStorage.begin(), _componentStorage.end(), c),
		_componentStorage.end());
	*/
}

void Entity::Destroy(bool force)
{
	// explicit or implicit instant change
	if (force || !isInActiveScene())
	{
		std::cout << "Destroying: " << this->GetID() << std::endl;

		// destroy all children 
		for (auto& e : _children)
			e->Destroy(true);

		// TODO: destruct all components 
		for (auto& c : _components)
			delete(c);

		// Remove from parent and release memory 
		if (_parent)
			_parent->removeChild(this->_id);

		delete(this);
	}
	else // defer
	{
		std::cout << "Scheduling: " << this->GetID() << " for destruction" << std::endl;
		OmegaEngine::Instance().DeferAction(
			new StatusActionParam(StatusActionType::Delete, this));
	}
}

void Entity::Release()
{
	// aka. murder_all_children_and_commit_suicide_but_remember_to_leave_a_note_for_your_parents()

	// 1. murder the children 
	for (int i = 0; i < _children.size(); i++)
	{
		_children[i]->Release();
	}

	// 2. leave a note 
	if (_parent) _parent->removeChild(_id);

	// 3. commit sudoku
	delete(this);
}

bool Entity::isInActiveScene() const
{
	return getScene() && getScene() == OmegaEngine::Instance().GetActiveScene();
}

void Entity::setScene(Scene * scene)
{
	_myScene = scene;
	for (auto& e : _children)
		e->setScene(scene);
}

Scene * Entity::getScene() const
{
	return _myScene;
}
