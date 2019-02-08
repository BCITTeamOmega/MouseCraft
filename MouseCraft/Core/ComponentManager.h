#pragma once
#include <algorithm>
#include <vector>
#include <memory>
#include "Component.h"
#include "../EventManager/ISubscriber.h"
#include "../EventManager/EventManager.h"

struct ComponentParams
{
	ComponentParams(Component* t) : target(t) {}
	Component* target;
};

template <class T>
class ComponentManager : public ISubscriber
{
public:

	static auto& Instance()
	{
		static ComponentManager<T> c;
		return c;
	}

	template<typename ComponentType, typename... Args>
	ComponentType* Create(Args... args)
	{
		static_assert(std::is_base_of<T, ComponentType>::value, "???");
		auto* t = new ComponentType(args...);
		_components.push_back(t);
		return t;
	}

	void Add(T* component) { _components.push_back(component); }
	void Remove(unsigned int id)
	{
		auto t = std::find_if(_components.begin(), _components.end(), [&id](const Component* c) { return c->getID() == id; });
		if(t != _components.end())
			_components.erase(t);
	}

	// Inherited via ISubscriber
	void Notify(EventName eventName, Param * params)
	{
		switch (eventName)
		{
			case COMPONENT_REMOVED:
			{
				auto* p = static_cast<TypeParam<Component*>*>(params);
				unsigned int t = p->Param->getID();
				Remove(t);
				break;
			}
			// lol rip the dream
			case COMPONENT_ADDED:
			{
				auto* p = static_cast<TypeParam<Component*>*>(params);
				auto* c = dynamic_cast<T*>(p->Param);
				if (c == nullptr)
					return;
				Add(c);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	const std::vector<T*>& All()
	{
		return _components;
	}

	ComponentManager(const ComponentManager&) = delete;
	ComponentManager& operator= (const ComponentManager) = delete;

private:
	std::vector<T*> _components;

protected:
	ComponentManager()
	{
		EventManager::Subscribe(COMPONENT_ADDED, this);
		EventManager::Subscribe(COMPONENT_REMOVED, this);
	}
	~ComponentManager()
	{
		EventManager::Unsubscribe(COMPONENT_ADDED, this);
		EventManager::Unsubscribe(COMPONENT_REMOVED, this);
	}
};