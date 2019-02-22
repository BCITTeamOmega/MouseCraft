#include "NetworkSystem.h"

#include <iostream>

using namespace std;

NetworkSystem::NetworkSystem() : _connection(192, 168, 0, 201, 88888) {
    if (!_socket.Open(DEFAULT_PORT)) {
        cerr << "Failed to create socket" << endl;
    }
}

NetworkSystem::~NetworkSystem() {
    _socket.Close();
}

void NetworkSystem::Update(float dt) {
    _tickCount += dt;
    if (_tickCount >= TICK_RATE) {
        serverTick();
        _tickCount -= TICK_RATE;
    }
}

void NetworkSystem::serverTick() {
    unsigned char buffer[MAX_PACKET_SIZE];

    Address sender;
    int received;

    while ((received = _socket.Receive(sender, buffer, MAX_PACKET_SIZE)) > 0) {
        processPacket(sender, buffer, received);
    }

    char message[] = "Test Packet";
    _dataSize = sizeof(message);

    _socket.Send(_connection, message, _dataSize);

    ++_tickNum;
}

void NetworkSystem::processPacket(Address &sender, unsigned char * data, const int size) const {
    cout << "[" <<
        sender.GetA() << "." <<
        sender.GetB() << "." <<
        sender.GetC() << "." <<
        sender.GetD() << ":" <<
        sender.GetPort() << "] " <<
        data << endl;
}