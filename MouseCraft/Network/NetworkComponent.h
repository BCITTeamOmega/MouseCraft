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

	virtual void OnInitialized() override;

    unsigned int GetNetworkID() const { return _netID; }
    const NetState & GetLastState() const { return _lastState; }
	
    const std::string GetComponentData() const;
    void AddComponentData(std::string& json);
	void AddComponentData(json json);
	void SetComponentData(json json);

	// Called by clients. Reconstructs all the components this entity should have. 
	void ConstructComponents();

    bool CheckDiff(unsigned short currTick);
    void StateUpdate(const NetState & ns);
private:
    bool moreRecent(unsigned short tick) {
        return (tick > _lastState.tick || (_lastState.tick - tick) > USHRT_MAX / 2);
    }

	json _componentData;
    NetState _lastState;
    unsigned int _netID;
    NetAuthority _authLevel;
};