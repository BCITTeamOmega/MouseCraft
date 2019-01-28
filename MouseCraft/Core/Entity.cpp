#include "Entity.h"
#include "OmegaEngine.h"
#include <iostream>

unsigned int Entity::_curID = 0;

Entity::Entity() :_id(++Entity::_curID) { }

Entity::Entity(Entity* parent) :_id(++Entity::_curID) 
{
	setParent(parent);
}

Entity::Entity(unsigned int id) : _id(id)
{
	std::cout << "Entity created with custom ID: " << _id << std::endl;
}

Entity::~Entity()
{
	std::cout << "Entity: " << _id << " destroyed" << std::endl;
}

unsigned int Entity::getID() const
{
	return _id;
}

void Entity::initialize()
{
	if (_initialized)
	{
		std::cout << "Entity already initialized" << std::endl;
		for (auto& c : _componentStorage)
		{
			c->initialize();
		}
	}

	_initialized = true;

	// (ensure) initialize children 
	for (auto& e : _children)
	{
		e->initialize();
	}
}

bool Entity::getActive() const
{
	return (_enabled && isInActiveScene() && getParentEnabled());
}

bool Entity::getEnabled() const
{
	return _enabled;
}

void Entity::setEnabled(bool enabled, bool force)
{
	// ensure actual change 
	if (_enabled == enabled) return;

	// defer? 
	if (!force && isInActiveScene())
	{
		// defer 
		return;
	}

	_enabled = enabled;
}

bool Entity::getStatic() const
{
	return _static;
}

void Entity::setStatic(bool torf)
{
	// todo: implement properly
	if (torf)
	{
		transform.computeLocalTransformation();
		transform.computeWorldTransformation();

		_static = true;				// freeze data 
		for (auto& e : _children)	// apply to all children 
			e->setStatic(true);
	}
	else
	{
		_static = false;
		for (auto& e : _children)
			e->setStatic(false);
	}
}

void Entity::setParent(Entity* parent, bool force)
{
	// ensure actual change
	if (parent == _parent) return;

	// defer ? 
	if (!force)
	{
		bool childOrParentInScene = isInActiveScene() || (parent != nullptr && parent->isInActiveScene());
		if (childOrParentInScene)
		{
			// raise event 
			return; 
		}
	}

	// execute 
	Entity::bindEntities(parent, this);
}

Entity* Entity::getParent() const
{
	return _parent;
}

void Entity::addChild(Entity* child, bool force)
{
	if (child == nullptr)
	{
		std::cerr << "ERROR: Entity::addChild() adding null child, please check your code!" << std::endl;
		return;
	}

	if (!force)
	{
		bool childOrParentInScene = isInActiveScene() || child->isInActiveScene();
		if (childOrParentInScene)
		{
			//defer
			return;
		}
	}

	Entity::bindEntities(this, child);
}

void Entity::removeChild(unsigned int id)
{
	auto t = std::find_if(_children.begin(), _children.end(), [&id](const Entity* e) { return e->getID() == id; });
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
		if (parent->isInActiveScene() && !child->_initialized)
		{
			child->initialize();
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

std::vector<Entity*> const& Entity::getChildren() const
{
	return _children;
}

void Entity::destroy(bool force)
{
	std::cerr << "ERROR: Entity::destroy() is not implemented yet" << std::endl;

	if (!force && isInActiveScene())
	{
		return;
	}
	
	delete(this);
}

void Entity::release()
{
	// aka. murder_all_children_and_commit_suicide_but_remember_to_leave_a_note_for_your_parents()

	// 1. murder the children 
	for (int i = 0; i < _children.size(); i++)
	{
		_children[i]->release();
	}

	// 2. leave a note 
	if (_parent) _parent->removeChild(_id);

	// 3. commit sudoku
	delete(this);
}

bool Entity::isInActiveScene() const
{
	return getScene() && getScene() == OmegaEngine::instance().activeScene;
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
