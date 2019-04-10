#pragma once
#include <glm/glm.hpp>
#include "../Core/Entity.h"

struct NetState {
    NetState() : tick(0) {}

    NetState(unsigned short tickNum, const Entity & entity);

    NetState(unsigned short tickNum, unsigned int parent, bool enable, glm::vec3 p, glm::vec3 r, glm::vec3 s) :
        tick(tickNum), parentID(parent), enabled(enable), pos(p), rot(r), scl(s) { }

    bool Equals(const NetState & ns) {
        return (pos == ns.pos && scl == ns.scl && rot == ns.rot && enabled == ns.enabled && parentID == ns.parentID);
    }

    unsigned short tick;
    unsigned int parentID;
    bool enabled;
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scl;
};

