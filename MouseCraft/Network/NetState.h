#pragma once
#include <glm/glm.hpp>
#include "../Core/Transform.h"

struct NetState {
    NetState() : tick(0) {}

    NetState(unsigned short tickNum, const Transform & t) {
        tick = tickNum;
        pos = t.getLocalPosition();
        rot = t.getLocalRotation();
        scl = t.getLocalScale();
    }

    bool Equals(const NetState & ns) {
        return (pos == ns.pos && scl == ns.scl && rot == ns.rot);
    }

    unsigned short tick;
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scl;
};

