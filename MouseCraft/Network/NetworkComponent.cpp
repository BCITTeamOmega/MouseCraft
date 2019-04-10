#include "NetworkComponent.h"

#include "NetworkSystem.h"
#include <numeric>
#include "../Loading/PrefabLoader.h"

NetworkComponent::~NetworkComponent() {
    if(_authLevel == AUTHORITATIVE)
        NetworkSystem::Instance()->DestroyEntityOnClients(this);
    NetworkSystem::Instance()->RemoveComponent(_netID);
}

void NetworkComponent::OnInitialized() {
    if (_authLevel == AUTHORITATIVE) {
        NetworkSystem::Instance()->SpawnEntityOnClients(this);
    }
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

void NetworkComponent::SetComponentData(std::string & json)
{
	_componentData = json::parse(json);
}

void NetworkComponent::SetComponentData(json json) {
	_componentData = json;
}

void NetworkComponent::ConstructComponents() {
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

bool NetworkComponent::CheckDiff(unsigned short currTick) {
    if (!moreRecent(currTick))
        return false;
    NetState next(currTick, GetEntity());
    if (!next.Equals(_lastState)) {
        _lastState = next;
        return true;
    }
    return false;
}

void NetworkComponent::StateUpdate(const NetState & ns) {
    if (ns.parentID != _lastState.parentID)
        NetworkSystem::Instance()->AddToEntity(ns.parentID, GetEntity());
    GetEntity()->SetEnabled(ns.enabled);
    GetEntity()->transform.setLocalPosition(ns.pos);
    GetEntity()->transform.setLocalRotation(ns.rot);
    GetEntity()->transform.setLocalScale(ns.scl);
}