#include "NetworkManager.h"
#include <iostream>

NetState NetworkManager::_state = NET_NO_INIT;
Socket NetworkManager::_socket;
NetRole NetworkManager::_role;
Address NetworkManager::_remote;

using namespace std;

bool NetworkManager::Initialize(const unsigned short portNum) {
    if (!_socket.Open(portNum)) {
        cerr << "Failed to create socket" << endl;
        return false;
    }

    _state = NET_STANDBY;
    return true;
}

void NetworkManager::CleanUp() {
    if (_state != NET_NO_INIT) {
        _socket.Close();
        _state = NET_NO_INIT;
    } else {
        cout << "Cleanup Failed. Network Manager not yet initialized" << endl;
    }
}

void NetworkManager::CreateRoom() {
    if (_state == NET_STANDBY) {
        _role = NET_SERVER;
        _state = NET_IN_ROOM;
    } else {
        cout << "Cannot Create Room in current state." << endl;
    }
}

void NetworkManager::CloseRoom() {

}

void NetworkManager::JoinServer(Address server) {
    if (_state == NET_STANDBY) {
        _remote = server;

        cout << "Joined " <<
            (int)server.GetA() << "." <<
            (int)server.GetB() << "." <<
            (int)server.GetC() << "." <<
            (int)server.GetD() << ":" <<
            server.GetPort() << endl;

        _role = NET_CLIENT;
        _state = NET_IN_ROOM;
    } else {
        cout << "Cannot Join Room in current state." << endl;
    }
}

void NetworkManager::LeaveServer() {

}

void NetworkManager::Update(const float delta) {
    switch (_state) {
    case NET_IN_GAME:
        break;
    case NET_IN_ROOM:
        if (_role == NET_SERVER) {
            unsigned char buffer[MAX_PACKET_SIZE];

            Address sender;
            int received = _socket.Receive(sender, buffer, sizeof(buffer));

            if (received > 0) {
                cout << "[" <<
                    (int)sender.GetA() << "." <<
                    (int)sender.GetB() << "." <<
                    (int)sender.GetC() << "." <<
                    (int)sender.GetD() << ":" <<
                    sender.GetPort() << "] " <<
                    buffer << endl;
            }
        } else {
            cin.clear();

            char message[MAX_PACKET_SIZE];
            cin.getline(message, MAX_PACKET_SIZE);

            if (!_socket.Send(_remote, message, sizeof(message))) {
                cout << "Packet send failure" << endl;
            }
        }
        break;
    case NET_STANDBY:
        break;
    default:
        break;
    }
}