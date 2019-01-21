#pragma once

#include <winsock2.h>
#pragma comment(lib, "WS2_32.lib")

enum NetState {
    NET_NO_INIT,
    NET_STANDBY,
    NET_IN_ROOM,
    NET_IN_GAME,
};

enum NetRole {
    NET_CLIENT,
    NET_SERVER,
};

class NetworkManager {
public:
    static const unsigned short PORT_NUMBER = 88888;
    static const unsigned int MAX_PACKET_SIZE = 256;

    static bool Initialize(const unsigned short portNum = PORT_NUMBER);
    static void CleanUp();

    static void CreateRoom();
    static void CloseRoom();
    static void JoinServer();
    static void LeaveServer();

    static void Update(const float delta);
private:
    static NetState     _state;
    static NetRole      _role;
    static WSADATA      _wsaData;
    static SOCKET       _socket;
    static SOCKADDR_IN  _localAddress;

    NetworkManager() {}
};

