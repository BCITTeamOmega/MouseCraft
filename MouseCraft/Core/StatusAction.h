#pragma once

// #include "Entity.h"

class Entity;

// Consider: Making a nested class and then having the engine have functions to interface.
// A command object that changes an entity's status when executed (add/delete/move/enable/disable)
class StatusAction
{
public:
	StatusAction(Entity* target, Entity* destination)
		: target(target), destination(destination) {}

	virtual void Execute() = 0;

protected:
	Entity* target;
	Entity* destination;
};

class MoveAction : StatusAction
{
	void Execute() override;
};

class DeleteAction : StatusAction
{
	void Execute() override;
};

class EnableAction : StatusAction
{
	void Execute() override;
};

class DisableAction : StatusAction
{
	void Execute() override;
};