#include "Entity.h"
#include "OmegaEngine.h"
#include <iostream>

unsigned int Entity::_curID = 0;

Entity::Entity() :_id(++Entity::_curID) { }

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
	if (!_initialized)
	{
		std::cout << "Entity already initialized" << std::endl;
		
		for (auto& c : _componentStorage)
		{
			c->initialize();
		}
		_initialized = true;
	}

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

	// explicit or implicit instant change  
	if (force || !isInActiveScene())
	{
		_enabled = enabled;
	}
	else // defer 
	{
		OmegaEngine::instance().deferAction(
			new StatusActionParam((enabled) ? StatusActionType::Enable : StatusActionType::Disable, this));
	}
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
	if (parent == this) return;

	// explicit or implicit instant change  
	bool childOrParentInScene = isInActiveScene() || (parent != nullptr && parent->isInActiveScene());
	if (force || !childOrParentInScene)
	{
		Entity::bindEntities(parent, this);
	}
	else // defer 
	{
		OmegaEngine::instance().deferAction(
			new StatusActionParam(StatusActionType::Move, this, parent));
	}
}

Entity* Entity::getParent() const
{
	return _parent;
}

// Warning: this function does not check if this child has already been added. 
void Entity::addChild(Entity* child, bool force)
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
		OmegaEngine::instance().deferAction(
			new StatusActionParam(StatusActionType::Move, child, this));
	}
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
		if (parent->isInActiveScene())
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

void Entity::addComponent(Component * component)
{
	component->setEntity(this);
	_componentStorage.push_back(component);
}

void Entity::destroy(bool force)
{
	std::cerr << "WARNING: Entity::destroy() is not fully implemented yet" << std::endl;

	// explicit or implicit instant change
	if (force || !isInActiveScene())
	{
		// TODO: destruct all components 
		for (auto& c : _componentStorage)
			delete(c);
		
		if (_parent)
			_parent->removeChild(this->_id);

		delete(this);
	}
	else // defer
	{
		OmegaEngine::instance().deferAction(
			new StatusActionParam(StatusActionType::Delete, this));
	}
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
	auto f = getScene();
	auto o = OmegaEngine::instance().getActiveScene();
	return getScene() && getScene() == OmegaEngine::instance().getActiveScene();
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
