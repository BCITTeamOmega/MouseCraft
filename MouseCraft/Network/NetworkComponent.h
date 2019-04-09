#pragma once
#include "../Core/Component.h"
#include "../Core/Entity.h"
#include <queue>
#include "NetState.h"

class NetworkComponent : public Component {
public:
    enum NetAuthority {
        SIMULATED,
        AUTHORITATIVE,
    };

    NetworkComponent(unsigned int netID, NetAuthority auth) : _netID(netID), _authLevel(auth) { }

    unsigned int GetNetworkID() const { return _netID; }
    const NetState & GetLastState() const { return _lastState; }

    bool CheckDiff(unsigned short currTick) {
        if (!moreRecent(currTick))
            return false;
        NetState next(currTick, GetEntity()->transform);
        if (!next.Equals(_lastState)) {
            _lastState = next;
            return true;
        }
        return false;
    }

    void StateUpdate(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
        GetEntity()->transform.setLocalPosition(pos);
        GetEntity()->transform.setLocalRotation(rot);
        GetEntity()->transform.setLocalScale(scl);
    }
private:
    bool moreRecent(unsigned short tick) {
        return (tick > _lastState.tick || (_lastState.tick - tick) > USHRT_MAX / 2);
    }

    NetState _lastState;
    unsigned int _netID;
    NetAuthority _authLevel;
};