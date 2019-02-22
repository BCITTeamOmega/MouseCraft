#pragma once

class Address {
public:
    Address();
    Address(const unsigned char a, const unsigned char b, const unsigned char c, const unsigned char d, const unsigned short port);
    Address(const unsigned int address, const unsigned short port);

    unsigned int GetAddress() const;

    unsigned char GetA() const;
    unsigned char GetB() const;
    unsigned char GetC() const;
    unsigned char GetD() const;

    unsigned short GetPort() const;
private:
    unsigned int _address;
    unsigned short _port;
};