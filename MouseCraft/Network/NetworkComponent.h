#pragma once
#include "../Core/Component.h"
#include <random>

class NetworkComponent : public Component {
public:
    enum NetAuthority {
        SIMULATED,
        AUTHORITATIVE,
        PLAYER
    };

    // Overrides Component::OnInitialized()
    void OnInitialized() override {
        std::random_device rd;
        std::mt19937 eng(rd);
        std::uniform_int_distribution<unsigned int> dist;

        _netID = dist(eng);
    }

    unsigned int GetNetworkID() const { return _netID; };
private:
    unsigned int _netID;
};