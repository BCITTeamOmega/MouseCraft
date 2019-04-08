#pragma once

#include "Address.h"

class Socket {
public:
    Socket();
    ~Socket();

    bool Open(const unsigned short port);
    void Close();

    bool Send(const Address &destination, const void *data, const int size);
    int Receive(Address &sender, void *buffer, const int size);
private:
    int _sockHandle;
};

