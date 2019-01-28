#pragma once

class Entity;

// Consider: Making a nested class and then having the engine have functions to interface.
// A command object that changes an entity's status when executed (add/delete/move/enable/disable)
class StatusAction
{
public:
	StatusAction(Entity* target, Entity* destination = nullptr)
		: target(target), destination(destination) {}

	virtual void Execute() = 0;

protected:
	Entity* target;
	Entity* destination;
};

class MoveAction : StatusAction
{
	MoveAction(Entity* target, Entity* destination = nullptr)
		: StatusAction(target, destination) {}
	void Execute() override;
};

class DeleteAction : StatusAction
{
	DeleteAction(Entity* target)
		: StatusAction(target) {}
	void Execute() override;
};

class EnableAction : StatusAction
{
	EnableAction(Entity* target)
		: StatusAction(target) {}
	void Execute() override;
};

class DisableAction : StatusAction
{
	DisableAction(Entity* target)
		: StatusAction(target) {}
	void Execute() override;
};