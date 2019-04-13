#include "NetworkSystem.h"

#include "../Core/ComponentManager.h"
#include "../Input/InputSystem.h"
#include "../Core/OmegaEngine.h"
#include "../ClientScene.h"
#include "NetState.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <random>

using namespace std;

NetworkSystem * NetworkSystem::_instance = nullptr;

NetworkSystem * NetworkSystem::Instance() {
    if (_instance == nullptr) {
        _instance = new NetworkSystem();
    }
    return _instance;
}

NetworkComponent * NetworkSystem::CreateComponent() {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<unsigned int> dist;
    unsigned int id;

    do {
        id = dist(eng);
    } while (_componentList.find(id) != _componentList.end() && id < 6);

    return CreateComponent(id);
}

NetworkComponent * NetworkSystem::CreateComponent(unsigned int netID) {
    NetworkComponent::NetAuthority auth = _role == HOST ? NetworkComponent::NetAuthority::AUTHORITATIVE : NetworkComponent::NetAuthority::SIMULATED;

    NetworkComponent *comp = ComponentManager<NetworkComponent>::Instance().Create<NetworkComponent, unsigned int, NetworkComponent::NetAuthority>(netID, auth);
    _componentList[netID] = comp;

    return comp;
}

NetworkSystem::NetworkSystem(const Role role, const unsigned short port) : _tickCount(0), _role(role) {
    unsigned short portNum = port;
    while (!_socket.Open(portNum)) {
        cerr << "Failed to create socket on " << portNum << ", trying " << ++portNum << endl;
    }
    EventManager::Subscribe(EventName::INPUT_BUTTON, this);
    EventManager::Subscribe(EventName::INPUT_AXIS_2D, this);
}

NetworkSystem::~NetworkSystem() {
    EventManager::Unsubscribe(EventName::INPUT_AXIS_2D, this);
    EventManager::Unsubscribe(EventName::INPUT_BUTTON, this);
    _socket.Close();
}

void NetworkSystem::RequestConnection(const Address & address) {
    if (_connectionList.find(address) == _connectionList.end()) {
        Connection conn(address);
        _connectionList.insert(std::pair<const Address, Connection>(address, conn));
    }
    _connectionList[address].Append(new ConnReqDatum());
}

void NetworkSystem::SearchForServers() { // Requests info from all computers on local network
    PacketData data;
    data.Append(new InfoReqDatum());
    _socket.Send(_broadcast, data.GetPointer(), data.GetSize());
    cout << "Looking for servers..." << endl;
}

void NetworkSystem::SetHost() {
    _role = Role::HOST;
}

void NetworkSystem::AddToEntity(unsigned int parentID, Entity * entity) {
    if (_componentList.find(parentID) != _componentList.end()) {
        entity->SetParent(_componentList[parentID]->GetEntity());
    } else {
        entity->SetParent(OmegaEngine::Instance().GetRoot());
    }
}

void NetworkSystem::SpawnEntityOnClients(NetworkComponent *component) {
//    appendToPackets(new EntityCreateDatum(component));
}

void NetworkSystem::DestroyEntityOnClients(NetworkComponent *component) {
//    appendToPackets(new EntityDestroyDatum(component));
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
            } else if(_role == CLIENT){
                appendToPackets(new PlayerButtonDatum(&data));
            }
        }
        break;
    }
    case EventName::INPUT_AXIS_2D: {
        if (_role == CLIENT) {
            auto data = static_cast<TypeParam<Axis2DEvent>*>(params)->Param;

            appendToPackets(new PlayerAxisDatum(&data));
        }
        break;
    }
    default:
        break;
    }
}

void NetworkSystem::RemoveComponent(unsigned int netID) {
    auto comp = _componentList.find(netID);
    if (comp != _componentList.end()) {
        _componentList.erase(comp);
    }
}

void NetworkSystem::serverTick() {
    // Receive and process any incoming packets
    Address sender;
    int received;

    while ((received = _socket.Receive(sender, _buffer, MAX_PACKET_SIZE)) > 0) {
        _rcv.Load(_buffer, received);
        if (_rcv.Verify())
            processPacket(sender, &_rcv);
    }

    // Append any state updates for all NetworkComponents
    if (_role == HOST) {
        for (auto component : _componentList) {
            if (component.first < 6 && component.second->CheckDiff(_tickNum)) {
                appendToPackets(new StateUpdateDatum(component.first, component.second->GetLastState()));
            }
        }
    }
    
    vector<Address> deleteList;

    // Send packet data to all active connections
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

void NetworkSystem::appendToPackets(NetDatum * datum) {
    for (auto & connection : _connectionList)
        if (connection.second.GetState() == Connection::State::LIVE)
            connection.second.Append(datum);
}

void NetworkSystem::processPacket(const Address &sender, PacketData *packet) {
    while (packet->HasNext())
        processDatum(sender, packet);

    if (_connectionList.find(sender) != _connectionList.end()) {
        _connectionList[sender].GotUpdate();
    }

    // Dump Packet data to console
    /*cout << "[" << sender << "] " <<
        packet->GetPointer() << '\0' << endl;*/
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
                _connectionList[sender].PlayerID = liveConnections();
                _connectionList[sender].SetLive();
                _connectionList[sender].Append(new ConnAccDatum(_tickNum));
                for (auto comp : _componentList) {
					if (comp.first > 5) { // Send all entities to newly connected players. Disabled for release build
						//_connectionList[sender].Append(new EntityCreateDatum(comp.second));
					}
                }
				_componentList[liveConnections()]->GetEntity()->SetEnabled(true);
            }
            _connectionList[sender].Append(new AckDatum(packet->GetTick()));
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
                _connectionList[sender].Append(new AckDatum(packet->GetTick()));
                _role = Role::CLIENT;
                _tickNum = newTick;
                OmegaEngine::Instance().ChangeScene(new ClientScene());
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
                _connectionList[sender].Append(new InfoResDatum(liveConnections()));
                _connectionList[sender].Append(new AckDatum(packet->GetTick()));
            }
            break;
        case NetDatum::DataType::HOST_INFO_RESPONSE: {
            unsigned short numPlayers = packet->ReadUShort();
            cout << "Server IP: " << sender << " Players: " << numPlayers + 1<< endl;
            if (_connectionList.find(sender) == _connectionList.end()) {
                Connection newConn(sender);
                _connectionList.insert(std::pair<const Address, Connection>(sender, newConn));
            }
            _connectionList[sender].Append(new AckDatum(packet->GetTick()));
            break;
        }
        case NetDatum::DataType::TRANSFORM_STATE_UPDATE:
            //cout << "Received State Update from " << sender << endl;
            if (_connectionList.find(sender) != _connectionList.end() && _connectionList[sender].GetState() == Connection::State::LIVE) {
                // Update state of NetworkComponent
                unsigned int componentID = packet->ReadUInt();

                unsigned int parent = packet->ReadUInt();
                bool enabled = packet->ReadByte();

                float posX = packet->ReadFloat();
                float posY = packet->ReadFloat();
                float posZ = packet->ReadFloat();

                float rotX = packet->ReadFloat();
                float rotY = packet->ReadFloat();
                float rotZ = packet->ReadFloat();

                float sclX = packet->ReadFloat();
                float sclY = packet->ReadFloat();
                float sclZ = packet->ReadFloat();

                NetState newState = {0, parent, enabled, glm::vec3(posX, posY, posZ), glm::vec3(rotX, rotY, rotZ), glm::vec3(sclX, sclY, sclZ) };

                if (_componentList.find(componentID) != _componentList.end()) {
                    _componentList[componentID]->StateUpdate(newState);
                } else {
                    cout << "State update from unknown component " << componentID << endl;
                }
            }
            break;
        case NetDatum::DataType::ENTITY_CREATE:
            if (_connectionList.find(sender) != _connectionList.end() && _connectionList[sender].GetState() == Connection::State::LIVE) {
                unsigned int netID = packet->ReadUInt();

                unsigned int parent = packet->ReadUInt();
                bool enabled = packet->ReadByte();

                float posX = packet->ReadFloat();
                float posY = packet->ReadFloat();
                float posZ = packet->ReadFloat();

                float rotX = packet->ReadFloat();
                float rotY = packet->ReadFloat();
                float rotZ = packet->ReadFloat();

                float sclX = packet->ReadFloat();
                float sclY = packet->ReadFloat();
                float sclZ = packet->ReadFloat();

                std::string componentData = packet->ReadString();
                
                Entity *newEntity = EntityManager::Instance().Create();
                newEntity->SetEnabled(enabled);
                newEntity->transform.setLocalPosition(glm::vec3(posX, posY, posZ));
                newEntity->transform.setLocalRotation(glm::vec3(rotX, rotY, rotZ));
                newEntity->transform.setLocalScale(glm::vec3(sclX, sclY, sclZ));

                NetworkComponent *newComponent = NetworkSystem::Instance()->CreateComponent(netID);
                newEntity->AddComponent(newComponent);

                newComponent->SetComponentData(componentData);
                newComponent->ConstructComponents();

                AddToEntity(parent, newEntity);

				_connectionList[sender].Append(new AckDatum(packet->GetTick()));
			}
            break;
        case NetDatum::DataType::ENTITY_DESTROY:
            if (_connectionList.find(sender) != _connectionList.end() && _connectionList[sender].GetState() == Connection::State::LIVE) {
                unsigned int netID = packet->ReadUInt();

                _componentList[netID]->GetEntity()->Destroy();

                _connectionList[sender].Append(new AckDatum(packet->GetTick()));
            }
            break;
        case NetDatum::DataType::EVENT_TRIGGER:
            if (_connectionList.find(sender) != _connectionList.end() && _connectionList[sender].GetState() == Connection::State::LIVE) {
                cout << "Received Event from " << sender << endl;
                _connectionList[sender].Append(new AckDatum(packet->GetTick()));
            }
            break;
        case NetDatum::DataType::PLAYER_AXIS:
            if (_connectionList.find(sender) != _connectionList.end() && _connectionList[sender].GetState() == Connection::State::LIVE) {
                Axis axis = (Axis)packet->ReadInt();
                float x = packet->ReadFloat();
                float y = packet->ReadFloat();
                glm::vec2 value(x, y);

				Axis2DEvent eventData{ _connectionList[sender].PlayerID, axis, value };
                EventManager::Notify(EventName::INPUT_AXIS_2D, new TypeParam<Axis2DEvent>(eventData));
            }
            break;
        case NetDatum::DataType::PLAYER_BUTTON:
            if (_connectionList.find(sender) != _connectionList.end() && _connectionList[sender].GetState() == Connection::State::LIVE) {
                Button button = (Button)packet->ReadInt();
                bool down = packet->ReadByte();

				ButtonEvent eventData{ _connectionList[sender].PlayerID, button, down };
                EventManager::Notify(EventName::INPUT_BUTTON, new TypeParam<ButtonEvent>(eventData));
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