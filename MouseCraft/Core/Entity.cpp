#include "Entity.h"

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

bool Entity::getActive() const
{
	if (!_enabled) 
		return false;
	if (getParent() == nullptr)
		return true;
	else
		return getParent()->getActive();
}

bool Entity::getEnabled() const
{
	return _enabled;
}

bool Entity::setEnabled(bool enabled)
{
	return _enabled = enabled;
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

void Entity::setParent(Entity* parent)
{
	Entity::bindEntities(parent, this);
}

Entity* Entity::getParent() const
{
	return _parent;
}

void Entity::addChild(Entity* child)
{
	Entity::bindEntities(this, child);
}

void Entity::removeChild(unsigned int id)
{
	auto t = std::find_if(_children.begin(), _children.end(), [&id](const Entity* e) { return e->getID() == id; });
	_children.erase(t);
}

void Entity::bindEntities(Entity * parent, Entity * child)
{
	if (!child) return;
	
	if (child->_parent)
		child->_parent->removeChild(child->_id);

	child->_parent = parent;

	if (parent)
		parent->_children.push_back(child);
}

std::vector<Entity*> const& Entity::getChildren() const
{
	return _children;
}



void Entity::destroy()
{
	std::cerr << "ERROR: Entity::destroy() is not implemented yet" << std::endl;
	// Game::instance().deleteEntity(this);
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
