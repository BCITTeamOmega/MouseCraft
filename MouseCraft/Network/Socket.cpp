#include "Socket.h"

#ifndef _WINSOCKAPI_
#include <winsock2.h>
#pragma comment(lib, "WS2_32.lib")
#endif

#include <iostream>

WORD winsockVersion = 0x202;

Socket::Socket() {
    WSADATA wsaData;
    if (WSAStartup(winsockVersion, &wsaData)) {
        std::cerr << "WSA Startup Failed: " << WSAGetLastError() << std::endl;
    }

    int addressFamily = AF_INET;
    int type = SOCK_DGRAM;
    int protocol = IPPROTO_UDP;
    _sockHandle = socket(addressFamily, type, protocol);
    if (_sockHandle == INVALID_SOCKET) {
        std::cerr << "Socket Creation Failed: " << WSAGetLastError() << std::endl;
    }

    const char broadcast = 1;
    setsockopt(_sockHandle, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
}

Socket::~Socket() {
    Close();
    WSACleanup();
}

bool Socket::Open(const unsigned short port) {
    SOCKADDR_IN local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(_sockHandle, (const SOCKADDR*)&local, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
        std::cerr << "Binding to Socket Failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    DWORD nonBlocking = 1;
    if (ioctlsocket(_sockHandle, FIONBIO, &nonBlocking) == SOCKET_ERROR) {
        std::cerr << "Failed to Set Non-Blocking: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

void Socket::Close() {
    if (closesocket(_sockHandle) == SOCKET_ERROR) {
        std::cerr << "Close Socket Failed: " << WSAGetLastError() << std::endl;
    }
}

bool Socket::Send(const Address &destination, const void *data, const int size) {
    SOCKADDR_IN address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(destination.GetAddress());
    address.sin_port = htons(destination.GetPort());

    int bytesSent = sendto(_sockHandle, (const char *)data, size, 0, (sockaddr*)&address, sizeof(SOCKADDR_IN));

    return (bytesSent == size);
}

int Socket::Receive(Address &sender, void *buffer, const int size) {
    SOCKADDR_IN from;
    int fromSize = sizeof(from);

    int received = recvfrom(_sockHandle, (char*)buffer, size, 0, (SOCKADDR*)&from, &fromSize);

    sender = Address(ntohl(from.sin_addr.s_addr), ntohs(from.sin_port));

    return received;
}