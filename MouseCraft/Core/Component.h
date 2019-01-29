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
	void initialize();
	bool getInitialized() const { return _initialized; }
	virtual void onInitialized() {};
	bool getEnabled() const { return _enabled; }
	void setEnabled(bool enabled) { _enabled = enabled; }
	Entity* getEntity() { return _entity; }
	void setEntity(Entity* e) { _entity = e; }	

private:
	bool _initialized = false;
	bool _enabled = true;
	Entity* _entity;
};