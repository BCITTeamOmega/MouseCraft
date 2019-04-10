#pragma once

#include "Address.h"
#include "PacketData.h"

#include <map>
#include <queue>

class Connection {
public:
    enum State {
        POTENTIAL,
        LIVE,
        INACTIVE,
        DEAD
    };

    Connection() {}
    Connection(Address location, State state = POTENTIAL) : _remote(location), _timeTillDeath(DEATH_TIME), _connState(state), PlayerID(-1) {}

    void GotUpdate() { 
		_timeTillDeath = DEATH_TIME;
		if (_connState == INACTIVE)
			_connState = LIVE;
	}

    void Tick() {
        _send.Clear();
		while (!_overflow.empty()) {
			_send.Append(_overflow.front());
			_overflow.pop();
		}
        if (--_timeTillDeath < 0) {
            _connState = (_connState == LIVE ? INACTIVE : DEAD);
        }
        _extraPacket = nullptr;
    }

    const State GetState() const { return _connState; }
    void SetLive() { _connState = Connection::State::LIVE; }

    const Address GetAddress() const { return _remote; }

    void SetTick(const unsigned short tickNum) { 
        _send.SetTick(tickNum);
        for (auto & packet : _reliableData) {
            if ((tickNum - packet.first) % RELIABILITY_RESEND == 0)
                _extraPacket = &packet.second;
        }
        if (_send.IsReliable())
            _reliableData.insert(std::pair<const unsigned short, PacketData>(tickNum, _send));
    }

    bool Append(const NetDatum * datum) { 
		if (_send.Append(datum)) {
			return true;
		}
		_overflow.push(datum);
		return false;
	}

    void Acknowledge(const unsigned short tickNum) { 
        int count = _reliableData.count(tickNum);
        while (count-- > 0) {
            _reliableData.erase(_reliableData.find(tickNum));
        }
    }

    const PacketData & GetPacket() const { return _send; }
    const PacketData * GetExtra() const { return _extraPacket; }

    int PlayerID;
private:
    const short DEATH_TIME = 600;
    const short RELIABILITY_RESEND = 600;

    short _timeTillDeath;
    Address _remote;
    State _connState;
    PacketData _send;
    std::multimap<const unsigned short, PacketData> _reliableData;
	std::queue<const NetDatum*> _overflow;
    PacketData * _extraPacket;
};