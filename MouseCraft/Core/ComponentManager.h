#pragma once
#include <vector>
#include <memory>
#include "Component.h"
#include "../EventManager/ISubscriber.h"

struct ComponentParams
{
	Component& target;
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

	template<typename... Args>
	T* Create(Args... args)
	{
		auto* t = new T(args...);
		_components.push_back(t);
		return t;
	}
	void Add(T& component) { _components.push_back(component); }
	void Remove(T& component) { _components.erase(std::find(_components.begin, _components.end, component)); }

	// Inherited via ISubscriber
	void Notify(EventName eventName, Param * params)
	{
		switch (eventName)
		{
			case COMPONENT_REMOVED:
			{
				auto* p = static_cast<TypeParam<ComponentParams*>*>(params);
				auto* c = p->Param;
				Remove(c->target);
				break;
			}
			case COMPONENT_ADDED:
			{
				auto* p = static_cast<TypeParam<ComponentParams*>*>(params);
				auto* c = p->Param;
				Add(c->target);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	ComponentManager(const ComponentManager&) = delete;
	ComponentManager& operator= (const ComponentManager) = delete;

private:
	std::vector<T*> _components;

protected:
	ComponentManager() {}
	~ComponentManager() {}
};