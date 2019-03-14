#include "NetworkSystem.h"

#include "../Input/InputSystem.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

NetworkSystem::NetworkSystem(const Role role, const unsigned short port) : _tickCount(0), _role(role) {
    unsigned short portNum = port;
    while (!_socket.Open(portNum)) {
        cerr << "Failed to create socket on " << portNum << ", trying " << ++portNum << endl;
    }
    EventManager::Subscribe(EventName::INPUT_BUTTON, this);
    EventManager::Subscribe(EventName::INPUT_AXIS, this);
}

NetworkSystem::~NetworkSystem() {
    EventManager::Unsubscribe(EventName::INPUT_AXIS, this);
    EventManager::Unsubscribe(EventName::INPUT_BUTTON, this);
    _socket.Close();
}

void NetworkSystem::RequestConnection(const Address & address) {
    if (_connectionList.find(address) == _connectionList.end()) {
        Connection conn(address);
        _connectionList.insert(std::pair<const Address, Connection>(address, conn));
    }
    _connectionList[address].Append(ConnReqDatum());
}

void NetworkSystem::SearchForServers() {
    PacketData data;
    data.Append(InfoReqDatum());
    _socket.Send(_broadcast, data.GetPointer(), data.GetSize());
    cout << "Looking for servers..." << endl;
}

void NetworkSystem::SetHost() {
    _role = Role::HOST;
}

void NetworkSystem::Update(float dt) {
    _tickCount += dt;
    if (_tickCount >= TICK_PERIOD) {
        serverTick();
        _tickCount -= TICK_PERIOD;
    }
}

void NetworkSystem::Notify(EventName name, Param * params) {
    switch (name) {
    case EventName::INPUT_BUTTON: {
        const char message[] = "Button Pressed";
        auto data = static_cast<TypeParam<ButtonEvent>*>(params)->Param;
        if (data.isDown) {
            if (data.button == Button::OPTION) {
                char buffer[256];
                cin.getline(buffer, sizeof(buffer));
                processInput(buffer);
            } else {
                //appendToPackets(message, sizeof(message));
            }
        }
        break;
    }
    case EventName::INPUT_AXIS: {
        const char message[] = "Axis Changed";
        //appendToPackets(message, sizeof(message));
        break;
    }
    default:
        break;
    }
}

void NetworkSystem::serverTick() {
    Address sender;
    int received;

    while ((received = _socket.Receive(sender, _buffer, MAX_PACKET_SIZE)) > 0) {
        _rcv.Load(_buffer, received);
        if (_rcv.Verify())
            processPacket(sender, &_rcv);
    }

    vector<Address> deleteList;

    for (auto & connection : _connectionList) {
        Connection * c = &connection.second;
        c->SetTick(_tickNum);
        if (!c->GetPacket().Empty() || _tickNum % POKE_TIME == 0) {
            _socket.Send(connection.first, c->GetPacket().GetPointer(), c->GetPacket().GetSize());
        }
        if (c->GetExtra() != nullptr) {
            _socket.Send(connection.first, c->GetExtra()->GetPointer(), c->GetExtra()->GetSize());
        }

        c->Tick();
        if (c->GetState() == Connection::State::DEAD)
            deleteList.push_back(connection.first);
    }

    // Delete inactive connections
    for (Address toDelete : deleteList) {
        cout << toDelete << " disconnected." << endl;
        _connectionList.erase(_connectionList.find(toDelete));
    }

    ++_tickNum;
}

void NetworkSystem::appendToPackets(const NetDatum & datum) {
    for (auto connection : _connectionList)
        if (connection.second.GetState() == Connection::State::LIVE)
            connection.second.Append(datum);
}

void NetworkSystem::processPacket(const Address &sender, PacketData *packet) {
    while (packet->HasNext())
        processDatum(sender, packet);

    if (_connectionList.find(sender) != _connectionList.end()) {
        _connectionList[sender].GotUpdate();
    }

    cout << "[" << sender << "] " <<
        packet->GetPointer() << '\0' << endl;
}

void NetworkSystem::processDatum(const Address &sender, PacketData *packet) {
    if (packet->HasNext()) {
        NetDatum::DataType type = static_cast<NetDatum::DataType>((int)packet->ReadByte());
        switch (type) {
        case NetDatum::DataType::ACK: {
            unsigned short tick = packet->ReadUShort();
            if (_connectionList.find(sender) != _connectionList.end()) {
                _connectionList[sender].Acknowledge(tick);
                cout << "Acknowledged packet " << tick << " from " << sender << endl;
            }
            break;
        }
        case NetDatum::DataType::CONNECTION_REQUEST: {
            cout << "Received Connection Request from " << sender << endl;
            if (_connectionList.find(sender) == _connectionList.end()) {
                Connection newConn(sender);
                _connectionList.insert(std::pair<const Address, Connection>(sender, newConn));
            }
            if (liveConnections() < maxConnections()) {
                cout << sender << " has connected." << endl;
                _connectionList[sender].SetLive();
                _connectionList[sender].Append(ConnAccDatum(_tickNum));
            }
            _connectionList[sender].Append(AckDatum(packet->GetTick()));
            break;
        }
        case NetDatum::DataType::CONNECTION_ACCEPT: {
            unsigned short newTick = packet->ReadUShort();
            if (_connectionList.find(sender) == _connectionList.end()) {
                Connection newConn(sender);
                _connectionList.insert(std::pair<const Address, Connection>(sender, newConn));
            }
            if (liveConnections() < maxConnections()) {
                cout << "Connection to " << sender << " accepted." << endl;
                _connectionList[sender].SetLive();
                _connectionList[sender].Append(AckDatum(packet->GetTick()));
                _role = Role::CLIENT;
                _tickNum = newTick;
            }
            break;
        }
        case NetDatum::DataType::HOST_INFO_REQUEST:
            cout << "Info requested by " << sender << endl;
            if (_role == Role::HOST) {
                if (_connectionList.find(sender) == _connectionList.end()) {
                    Connection newConn(sender);
                    _connectionList.insert(std::pair<const Address, Connection>(sender, newConn));
                }
                _connectionList[sender].Append(InfoResDatum(liveConnections()));
                _connectionList[sender].Append(AckDatum(packet->GetTick()));
            }
            break;
        case NetDatum::DataType::HOST_INFO_RESPONSE: {
            unsigned short numPlayers = packet->ReadUShort();
            cout << "Server IP: " << sender << " Players: " << numPlayers + 1<< endl;
            if (_connectionList.find(sender) == _connectionList.end()) {
                Connection newConn(sender);
                _connectionList.insert(std::pair<const Address, Connection>(sender, newConn));
            }
            _connectionList[sender].Append(AckDatum(packet->GetTick()));
            break;
        }
        case NetDatum::DataType::TRANSFORM_STATE_UPDATE:
            cout << "Received State Update from " << sender << endl;
            if (_connectionList.find(sender) != _connectionList.end() && _connectionList[sender].GetState() == Connection::State::LIVE) {
                // Update state of NetworkComponents
            }
            break;
        case NetDatum::DataType::EVENT_TRIGGER:
            if (_connectionList.find(sender) != _connectionList.end() && _connectionList[sender].GetState() == Connection::State::LIVE) {
                cout << "Received Event from " << sender << endl;
                _connectionList[sender].Append(AckDatum(packet->GetTick()));
            }
            break;
        default:
            cerr << "Unexpected NetDatum Type received from " << sender << endl;
            break;
        }
    }
}

int NetworkSystem::liveConnections() {
    int count = 0;
    for (auto connection : _connectionList)
        if (connection.second.GetState() == Connection::State::LIVE)
            ++count;
    return count;
}

int NetworkSystem::maxConnections() {
    return (_role == NetworkSystem::Role::HOST ? 3 : 1);
}

void NetworkSystem::processInput(string line) {
    istringstream iss(line);
    string first, option;
    iss >> first;
    if (first == "connect") {
        int a, b, c, d, p;
        getline(iss, option, '.');
        a = stoi(option);
        getline(iss, option, '.');
        b = stoi(option);
        getline(iss, option, '.');
        c = stoi(option);
        getline(iss, option, ':');
        d = stoi(option);
        getline(iss, option, ' ');
        p = stoi(option);

        RequestConnection(Address(a,b,c,d,p));
    } else if (first == "search") {
        SearchForServers();
    }
}