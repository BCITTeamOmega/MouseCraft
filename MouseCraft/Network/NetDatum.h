#pragma once

#include "../Util/TypePunners.h"
#include <algorithm>

constexpr size_t MAX_DATUM_SIZE = 128;

class NetDatum {
public:
    enum DataType {
        ACK = 0x00,
        CONNECTION_REQUEST = 0x01,
        CONNECTION_ACCEPT = 0x02,
        HOST_INFO_REQUEST = 0x03,
        HOST_INFO_RESPONSE = 0x04,
        TRANSFORM_STATE_UPDATE = 0x10,
        EVENT_TRIGGER = 0x20
    };

    const unsigned char GetType() const { return IntChar{_type}.c[0]; }
    const unsigned char * GetPointer() const { return _data; };
    const size_t GetSize() const { return _size; };

    virtual const bool IsReliable() const = 0;
protected:
    NetDatum(const DataType type, const size_t size) : _type(type), _size(size) {};

    DataType _type;

    size_t _size;
    unsigned char _data[MAX_DATUM_SIZE];
};

class AckDatum : public NetDatum {
public:
    AckDatum(const unsigned short tickNum) : NetDatum(NetDatum::ACK, sizeof(tickNum)) {
        UShortChar tick = { tickNum };
        std::copy(tick.c, tick.c + _size, _data);
    }

    const bool IsReliable() const override { return false; }
};

class ConnReqDatum : public NetDatum {
public:
    ConnReqDatum() : NetDatum(NetDatum::CONNECTION_REQUEST, 0) { }

    const bool IsReliable() const override { return true; }
};

class ConnAccDatum : public NetDatum {
public:
    ConnAccDatum(const unsigned short tickNum) : NetDatum(NetDatum::CONNECTION_ACCEPT, sizeof(tickNum)) { 
        UShortChar tick = { tickNum };
        std::copy(tick.c, tick.c + _size, _data);
    }

    const bool IsReliable() const override { return true; }
};

class InfoReqDatum : public NetDatum {
public:
    InfoReqDatum() : NetDatum(NetDatum::HOST_INFO_REQUEST, 0) { }

    const bool IsReliable() const override { return false; }
};

class InfoResDatum : public NetDatum {
public:
    InfoResDatum(const unsigned short numPlayers) : NetDatum(NetDatum::HOST_INFO_RESPONSE, sizeof(numPlayers)) {
        UShortChar players = { numPlayers };
        std::copy(players.c, players.c + _size, _data);
    }

    const bool IsReliable() const override { return false; }
};