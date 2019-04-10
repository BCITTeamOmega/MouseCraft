#include "NetState.h"

#include "NetworkComponent.h"
#include <iostream>

NetState::NetState(unsigned short tickNum, const Entity * entity) {
    tick = tickNum;
    if (entity != nullptr) {
        Transform t = entity->transform;
        pos = t.getLocalPosition();
        rot = t.getLocalRotation();
        scl = t.getLocalScale();
        enabled = entity->GetEnabled();

        Entity *parent = entity->GetParent();
        if (parent != nullptr) {
            NetworkComponent *comp = parent->GetComponent<NetworkComponent>();
            if (comp != nullptr)
                parentID = comp->GetNetworkID();
        }
    } else {
        std::cout << "Null Entity NetState Creation" << std::endl;
    }
}