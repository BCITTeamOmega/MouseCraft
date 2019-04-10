#pragma once
#include "../Core/Component.h"
#include "../Core/Entity.h"
#include <string>
#include <queue>
#include "NetState.h"

#include "../json.hpp"
#include "../Loading/PrefabLoader.h"
using json = nlohmann::json;

class NetworkComponent : public Component {
public:
    enum NetAuthority {
        SIMULATED,
        AUTHORITATIVE,
    };

    NetworkComponent(unsigned int netID, NetAuthority auth) : _netID(netID), _authLevel(auth) { }

	// TODO: please reivew implementation
	virtual void OnInitialized() override;

    unsigned int GetNetworkID() const { return _netID; }
    const NetState & GetLastState() const { return _lastState; }
	
	// TODO: please review implementation
	const std::string GetComponentData() const;
	void AddComponentData(std::string& json);
	void AddComponentData(json json);
	void SetComponentData(json json);

	// TODO: may want to move this into the system
	// Called by clients. Reconstructs all the components this entity should have. 
	void ConstructComponents();

    bool CheckDiff(unsigned short currTick) {
        if (!moreRecent(currTick))
            return false;
        NetState next(currTick, *GetEntity());
        if (!next.Equals(_lastState)) {
            _lastState = next;
            return true;
        }
        return false;
    }

    void StateUpdate(const NetState & ns) {
        if (ns.parentID != _lastState.parentID)
            NetworkSystem::Instance()->AddToEntity(ns.parentID, GetEntity());
        GetEntity()->SetEnabled(ns.enabled);
        GetEntity()->transform.setLocalPosition(ns.pos);
        GetEntity()->transform.setLocalRotation(ns.rot);
        GetEntity()->transform.setLocalScale(ns.scl);
    }
private:
    bool moreRecent(unsigned short tick) {
        return (tick > _lastState.tick || (_lastState.tick - tick) > USHRT_MAX / 2);
    }

	json _componentData;
    NetState _lastState;
    unsigned int _netID;
    NetAuthority _authLevel;
};