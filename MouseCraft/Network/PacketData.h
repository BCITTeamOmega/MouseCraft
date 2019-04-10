#pragma once

#ifndef _WINSOCKAPI_
#include <winsock2.h>
#pragma comment(lib, "WS2_32.lib")
#endif

#include "../Util/TypePunners.h"
#include "NetDatum.h"
#include <string>

constexpr size_t MAX_PACKET_SIZE = 256;

class PacketData {
public:
    PacketData();
    PacketData(const unsigned char * data, const size_t size);

    void Load(const unsigned char * data, const size_t size);

    bool Append(const char * string, const size_t length);
    bool Append(const NetDatum & datum);

    size_t GetData(unsigned char * buffer) const;

    const unsigned char * GetPointer() const;
    size_t GetSize() const;

    void SetTick(const unsigned short num);
    unsigned short GetTick() const;

    int ReadInt();
    unsigned int ReadUInt();
    short ReadShort();
    unsigned short ReadUShort();
    float ReadFloat();
    unsigned char ReadByte();
    std::string ReadString();

    bool Verify() const;
    bool Empty() const;
    bool HasNext() const;
    bool IsReliable() const;
    void Clear();
    void Reset();
private:
    const UIntChar PROTOCOL_ID = { htonl('MOUS') };
    const size_t HEADER_SIZE = sizeof(PROTOCOL_ID) + sizeof(UShortChar);

    unsigned char _data[MAX_PACKET_SIZE];
    size_t _size;
    
    size_t _readPos;
    bool _reliable;
};