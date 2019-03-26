#pragma once

#include "../Core/System.h"
#include "Address.h"
#include "Socket.h"
#include "PacketData.h"
#include "Connection.h"
#include "NetDatum.h"
#include "NetworkComponent.h"
#include <map>

#include "../Event/EventManager.h"

constexpr unsigned short DEFAULT_PORT = 8878;

class NetworkSystem : public System, public ISubscriber {
public:
    enum Role {
        HOST,
        CLIENT
    };

    static NetworkSystem * Instance();

    NetworkComponent * CreateComponent();

    void RequestConnection(const Address & address);
    void SearchForServers();
    void SetHost();

    //Overrides System::Update
    void Update(float dt) override;

    //Overrides ISubscriber::Notify
    void Notify(EventName eventName, Param *params) override;
private:
    NetworkSystem(const Role role = Role::HOST, const unsigned short port = DEFAULT_PORT);
    ~NetworkSystem();

    const int TICK_RATE = 30;
    //TODO: change back to 1.f once delta time fixed by Jason
    const float TICK_PERIOD = .1f / TICK_RATE;
    const int POKE_TIME = 3 * TICK_RATE;

    void serverTick();

    void appendToPackets(const NetDatum & datum);

    void processPacket(const Address & sender, PacketData * packet);
    void processDatum(const Address & sender, PacketData * packet);

    int liveConnections();
    int maxConnections();

    void processInput(std::string line);

    PacketData _rcv;
    unsigned char _buffer[MAX_PACKET_SIZE];

    unsigned short _tickNum;
    float _tickCount;
    Socket _socket;
    std::map<const Address, Connection> _connectionList;
    Role _role;
    const Address _broadcast = Address(255, 255, 255, 255, DEFAULT_PORT);

    std::map<unsigned int, NetworkComponent*> _componentList;

    static NetworkSystem *_instance;
};

