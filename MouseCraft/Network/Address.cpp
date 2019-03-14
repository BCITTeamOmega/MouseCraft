#include "Address.h"

Address::Address()
    : _address(0), _port(0) {}

Address::Address(const unsigned char a, const unsigned char b, const unsigned char c, const unsigned char d, const unsigned short port)
    : _port(port) {
    _address = (a << 24) | (b << 16) | (c << 8) | d;
}

Address::Address(const unsigned int address, const unsigned short port)
    : _address(address), _port(port) {}

unsigned int Address::GetAddress() const { return _address; }

unsigned char Address::GetA() const { return _address >> 24; }
unsigned char Address::GetB() const { return (_address >> 16) & 0x000000FF; }
unsigned char Address::GetC() const { return (_address >> 8) & 0x000000FF; }
unsigned char Address::GetD() const { return _address & 0x000000FF; }

unsigned short Address::GetPort() const { return _port; }

bool Address::operator <(const Address& rhs) const {
    return GetAddress() < rhs.GetAddress();
}

std::ostream& operator<<(std::ostream &strm, const Address &a) {
    return strm << (int)a.GetA() << "." << (int)a.GetB() << "." << (int)a.GetC() << "." << (int)a.GetD() << ":" << a.GetPort();
}