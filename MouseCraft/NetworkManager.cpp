#include "NetworkManager.h"
#include <iostream>

WORD winsockVersion = 0x202;

NetState NetworkManager::_state = NET_NO_INIT;
WSADATA NetworkManager::_wsaData;
SOCKET NetworkManager::_socket;
SOCKADDR_IN NetworkManager::_localAddress;
NetRole NetworkManager::_role;

using namespace std;

bool NetworkManager::Initialize(const unsigned short portNum) {
    if (WSAStartup(winsockVersion, &_wsaData)) {
        cout << "WSA Startup Failed: " << WSAGetLastError() << endl;
        return false;
    }

    int addressFamily = AF_INET;
    int type = SOCK_DGRAM;
    int protocol = IPPROTO_UDP;
    _socket = socket(addressFamily, type, protocol);
    if (_socket == INVALID_SOCKET) {
        cout << "Socket Creation Failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return false;
    }

    _localAddress.sin_family = AF_INET;
    _localAddress.sin_port = htons(portNum);
    _localAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(_socket, (SOCKADDR*)&_localAddress, sizeof(_localAddress)) == SOCKET_ERROR) {
        cout << "Binding to Socket Failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return false;
    }

    DWORD nonBlocking = 1;
    if (ioctlsocket(_socket, FIONBIO, &nonBlocking) == SOCKET_ERROR) {
        cout << "Failed to set non-blocking: " << WSAGetLastError() << endl;
        WSACleanup();
        return false;
    }

    _state = NET_STANDBY;
    return true;
}

void NetworkManager::CleanUp() {
    if (_state != NET_NO_INIT) {
        if (closesocket(_socket) == SOCKET_ERROR) {
            cout << "Close Socket Failed: " << WSAGetLastError() << endl;
        }
        WSACleanup();
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

void NetworkManager::JoinServer() {

}

void NetworkManager::LeaveServer() {

}

void NetworkManager::Update(const float delta) {
    char buffer[MAX_PACKET_SIZE];
    switch (_state) {
    case NET_IN_GAME:
        break;
    case NET_IN_ROOM:
        if (_role == NET_SERVER) {
            SOCKADDR_IN from;
            int fromSize = sizeof(from);
            int received = recvfrom(_socket, buffer, MAX_PACKET_SIZE, 0, (SOCKADDR*)&from, &fromSize);

            if (received == SOCKET_ERROR) {
                cout << "Receiving from socket failed: " << WSAGetLastError() << endl;
            } else if (received > 0) {
                buffer[received] = '\n';
                cout << "[" << 
                    from.sin_addr.S_un.S_un_b.s_b1 << "." <<
                    from.sin_addr.S_un.S_un_b.s_b2 << "." <<
                    from.sin_addr.S_un.S_un_b.s_b3 << "." <<
                    from.sin_addr.S_un.S_un_b.s_b4 << ":" <<
                    ntohs(from.sin_port) << "] " <<
                    buffer << endl;
            }
        } else {

        }
        break;
    case NET_STANDBY:
        break;
    default:
        break;
    }
}