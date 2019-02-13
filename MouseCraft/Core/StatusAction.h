#pragma once

// Note: You should not use any of these. These are used internally by the engine. 

class Entity;

enum StatusActionType
{
	Move, Delete, Enable, Disable
};

struct StatusActionParam
{
public:
	StatusActionParam(StatusActionType action, Entity* target, Entity* destination = nullptr)
		: action(action), target(target), destination(destination) {}

	StatusActionType action;
	Entity* target;
	Entity* destination;
};

/*
// Removing command objects - let engine decide how to execute the action. 
// This is important b/c the current implementation the engine uses a deque to put delete on the back, else on front.
// With another implementation it could possibly use two queues (perserve 'calling order').

// Leaving this here as a reference.

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
*/