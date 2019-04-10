#include "PacketData.h"
#include <algorithm>

PacketData::PacketData() : _size(HEADER_SIZE), _readPos(HEADER_SIZE), _reliable(false) {
    std::copy(PROTOCOL_ID.c, PROTOCOL_ID.c + sizeof(PROTOCOL_ID), _data);
}

PacketData::PacketData(const unsigned char * data, const size_t size) : _size(size), _readPos(HEADER_SIZE), _reliable(false) {
    std::copy(data, data + size, _data);
}

void PacketData::Load(const unsigned char * data, const size_t size) {
    std::copy(data, data + size, _data);
    _size = size;
    _readPos = HEADER_SIZE;
}

bool PacketData::Append(const char * string, const size_t length) {
    if (_size + length < MAX_PACKET_SIZE) {
        std::copy(string, string + length, _data + _size);
        _size += length;

        //_data[_size++] = '\0';

        return true;
    }
    return false;
}

bool PacketData::Append(const NetDatum * datum) {
    if (_size + datum->GetSize() + 1 < MAX_PACKET_SIZE) {
        _data[_size++] = datum->GetType();
        if (datum->IsReliable())
            _reliable = true;
        std::copy(datum->GetPointer(), datum->GetPointer() + datum->GetSize(), _data + _size);
        _size += datum->GetSize();
		delete datum;
        return true;
    }
	delete datum;
    return false;
}

size_t PacketData::GetData(unsigned char * buffer) const {
    std::copy(_data, _data + _size, buffer);

    return _size;
}

const unsigned char * PacketData::GetPointer() const {
    return _data;
}

size_t PacketData::GetSize() const {
    return _size;
}

bool PacketData::Verify() const {
    for (size_t i = 0; i < sizeof(PROTOCOL_ID); ++i)
        if (_data[i] != PROTOCOL_ID.c[i])
            return false;
    return true;
}

void PacketData::SetTick(const unsigned short num) {
    UShortChar tick;
    tick.s = htons(num);
    std::copy(tick.c, tick.c + sizeof(tick), _data + sizeof(PROTOCOL_ID));
}

unsigned short PacketData::GetTick() const {
    UShortChar tick;
    std::copy(_data + sizeof(PROTOCOL_ID), _data + sizeof(PROTOCOL_ID) + sizeof(tick), tick.c);
    return ntohs(tick.s);
}

int PacketData::ReadInt() {
    IntChar d;
    std::copy(_data + _readPos, _data + _readPos + sizeof(d), d.c);
    _readPos += sizeof(d);
    return d.i;
}

unsigned int PacketData::ReadUInt() {
    UIntChar d;
    std::copy(_data + _readPos, _data + _readPos + sizeof(d), d.c);
    _readPos += sizeof(d);
    return d.i;
}

short PacketData::ReadShort() {
    ShortChar d;
    std::copy(_data + _readPos, _data + _readPos + sizeof(d), d.c);
    _readPos += sizeof(d);
    return d.s;
};

unsigned short PacketData::ReadUShort() {
    UShortChar d;
    std::copy(_data + _readPos, _data + _readPos + sizeof(d), d.c);
    _readPos += sizeof(d);
    return d.s;
};

float PacketData::ReadFloat() {
    FloatChar d;
    std::copy(_data + _readPos, _data + _readPos + sizeof(d), d.c);
    _readPos += sizeof(d);
    return d.f;
}

unsigned char PacketData::ReadByte() {
    return _data[_readPos++];
}

std::string PacketData::ReadString() {
    unsigned int size = ReadUInt();
    char * str = (char*)malloc(size * sizeof(char));
    std::copy(_data + _readPos, _data + _readPos + size, str);
    std::string ret(str, size);
    _readPos += size;
    free(str);
    return ret;
}

bool PacketData::Empty() const {
    return _size == HEADER_SIZE;
}

bool PacketData::HasNext() const {
    return _readPos < _size;
}

bool PacketData::IsReliable() const {
    return _reliable;
}

void PacketData::Clear() {
    Reset();
    ZeroMemory(_data + HEADER_SIZE, MAX_PACKET_SIZE - HEADER_SIZE);
    _size = HEADER_SIZE;
    _reliable = false;
}

void PacketData::Reset() {
    _readPos = HEADER_SIZE;
}