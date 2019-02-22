#pragma once

#include "../Core/System.h"
#include "Address.h"
#include "Socket.h"

#define MAX_PACKET_SIZE 256

class NetworkSystem : System {
public:
    NetworkSystem();
    ~NetworkSystem();

    void Update(float dt);
private:
    const unsigned short DEFAULT_PORT = 88888;
    const float TICK_RATE = 1.f / 60;

    void serverTick();

    void processPacket(Address & sender, unsigned char * data, const int size) const;

    unsigned char _packetData[MAX_PACKET_SIZE];
    unsigned int _dataSize = 0;

    unsigned short _tickNum;
    float _tickCount;
    Socket _socket;
    Address _connection;
};

