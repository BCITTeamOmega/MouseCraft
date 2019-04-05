#pragma once
#include "../Core/Component.h"

class NetworkComponent : public Component {
public:
    enum NetAuthority {
        SIMULATED,
        AUTHORITATIVE,
    };

    NetworkComponent(unsigned int netID, NetAuthority auth) : _netID(netID), _authLevel(auth) { }

    unsigned int GetNetworkID() const { return _netID; };
private:
    unsigned int _netID;
    NetAuthority _authLevel;
};