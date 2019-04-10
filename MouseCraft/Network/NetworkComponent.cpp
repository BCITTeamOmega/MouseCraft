#include "NetworkComponent.h"
#include <numeric>

const std::string NetworkComponent::GetComponentData() const {
	return _componentData.dump();
}

void NetworkComponent::AddComponentData(std::string & json) { 
	_componentData.push_back(json::parse(json));
}

void NetworkComponent::AddComponentData(json json) {
	_componentData.push_back(json);
}
