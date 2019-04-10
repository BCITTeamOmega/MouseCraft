#include "NetworkComponent.h"
#include <numeric>
#include "../Loading/PrefabLoader.h"

void NetworkComponent::OnInitialized()
{
	// 0. Ensure _componentData is set (this should be fine, dw about it).
	// 1. If server send clients "create entity" w/ "component data"
	// 2. If client call ConstructComponents(); // altneratively calling ConstructComponents() via a system call would be better right when this is created.
}

const std::string NetworkComponent::GetComponentData() const {
	return _componentData.dump();
}

void NetworkComponent::AddComponentData(std::string & json) { 
	_componentData.push_back(json::parse(json));
}

void NetworkComponent::AddComponentData(json json) {
	_componentData.push_back(json);
}

void NetworkComponent::SetComponentData(json json)
{
	_componentData = json;
}

void NetworkComponent::ConstructComponents()
{
	// construct each component 
	for (auto& j : _componentData)
	{
		if (j["type"].get<std::string>() == "file")
		{
			// via prefab component file 
			auto c = PrefabLoader::LoadComponent(j["value"].get<std::string>());
			GetEntity()->AddComponent(c);
		}
		else
		{
			// via raw json
			std::cout << "ERROR: NetworkComponent only handles file component data at the moment." << std::endl;
		}
	}
}
