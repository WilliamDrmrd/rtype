# Engine/Server Technical Documentation

## ClientInfo.hpp
```cpp
#pragma once

#include <SFML/Network.hpp>

enum class ClientState {
    InLobby,
    InGame,
    Disconnected
};

struct ClientInfo {
        sf::IpAddress  address;
        unsigned short port;
        ClientState    state;
        bool           hasSwitchedWorld = false;
        bool           isInitialized    = false;
        bool           isServer         = false;
        int            nbPlayer         = 0;

        ClientInfo() : port(0), state(ClientState::Disconnected) {} // Constructeur par défaut

        ClientInfo(const sf::IpAddress &addr, unsigned short prt, ClientState st, bool isServer)
            : address(addr), port(prt), state(st), isServer(isServer)
        {
        }
};
```

### **`ClientState` Enumeration:**

- **Purpose**: Represents the different states a client can be in.
- **Enumerators**:
    - **`InLobby`**: Indicates that the client is currently in the lobby.
    - **`InGame`**: Indicates that the client is in an active game.
    - **`Disconnected`**: Indicates that the client is disconnected.

### **`ClientInfo` Structure:**

- **Purpose**: Contains information about a client.
- **Members**:
    - **`address`**: An instance of **`sf::IpAddress`** representing the client's IP address.
    - **`port`**: An unsigned short representing the client's port.
    - **`state`**: An instance of **`ClientState`** representing the client's state (InLobby, InGame, Disconnected).
    - **`hasSwitchedWorld`**: A boolean indicating whether the client has switched to a different game world.
    - **`isInitialized`**: A boolean indicating whether the client is initialized.
    - **`isServer`**: A boolean indicating whether the client is acting as a server.
    - **`nbPlayer`**: An integer representing the number of players (or clients) associated with the client.
- **Constructors**:
    - Default Constructor: Initializes **`port`** to 0 and **`state`** to **`ClientState::Disconnected`**.
    - Parameterized Constructor: Takes **`addr`** (sf::IpAddress), **`prt`** (unsigned short), **`st`** (**`ClientState`**), and **`isServer`** (bool) to initialize the corresponding members.

## Network.hpp
```cpp
#pragma once

#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include <thread>

#include "ECS/Components.hpp"
#include "Engine/Server/WaitingRoom.hpp"
#include "public/AvailableComponents.hpp"

enum class PacketType {
    HandshakeRequest,
    HandshakeResponse,
    SwitchWorld,
    SwitchWorldOkForMe,
    LeaveLobby,
    LeaveLobbyResponse,
    ClientIndependentInitialization,
    InitializeGame,
    InitializeGameOkForMe,
    LaunchGame,
    KeyInputs,
    ClientUpdate,
    ClientUpdateACK,
    GlobalState,
    PlayerAction,
    ActionOutcome,
    Heartbeat,
    PlayerDisconnected,
    Error,
    ErrorAcknowledged,
    EndGame,
    EndGameAcknowledged,
};

enum class UpdateType {
    AddComponents,
    RemoveComponents,
    RemoveEntity
};

struct IpAddressHash {
        std::size_t operator()(const sf::IpAddress &ip) const
        {
            return std::hash<std::string>()(ip.toString());
        }
};
namespace ECS
{
    class Network
    {
        public:
            Network() = default;

            // =========================================================
            // ==================== SERVER HANDLER =====================
            // =========================================================

            void startServer(unsigned short port);
            void startClient(unsigned short port);

            void connectToServer(const sf::IpAddress &serverAddress, unsigned short port);
            void sendConnectionToServer(const sf::IpAddress &serverAddress, unsigned short port);

            void stop();

            // =========================================================
            // ======================= ENVOI ===========================
            // =========================================================

            void sendUpdatedEntitiesToClients();
            void sendRemovedComponentsToClients();
            void sendRemovedEntitiesToClients(std::vector<ECS::id_t> &removedEntities);
            void sendEventsToServer();

            int getNbEntitiesModified();
            int getNbEntitiesWithDeletedComponents();

            void addSerializedEventToPacket(sf::Packet &packet, sf::Event event);
            void addSerializedComponentToPacket(sf::Packet &packet, ECS::BaseComponent *component);
            void addSerializedEntityToPacket(sf::Packet                                                     &packet,
                                             const std::pair<const ECS::id_t, std::unique_ptr<ECS::Entity>> &pair);
            void
                 addSerializedDeletedEntityToPacket(sf::Packet                                                     &packet,
                                                    const std::pair<const ECS::id_t, std::unique_ptr<ECS::Entity>> &pair);
            void sendPacketTypeToServer(PacketType packetType, const sf::IpAddress &recipient, unsigned short port);
            void sendPacketToServer(sf::Packet &packet);

            void sendPacketToAllClients(sf::Packet &packet, bool includeServer = true);
            void sendPacketToClient(sf::Packet &packet, const sf::IpAddress &address, unsigned short port);

            // =========================================================
            // ====================== RECEPTION ========================
            // =========================================================

            void      receivePackets();
            void      handleReceivedPacket(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort);
            void      deserializeEntityAndApply(sf::Packet &packet);
            void      deserializeRemoveEntitiesAndRemove(sf::Packet &packet);
            void      deserializeRemovedComponentsAndApply(sf::Packet &packet);
            sf::Event deserializeEvent(sf::Packet &packet);

            // =========================================================
            // ======================= GETTERS =========================
            // =========================================================

            sf::UdpSocket &getSocket()
            {
                return socket;
            }
            sf::IpAddress getIP() const
            {
                return serverAddress;
            }
            int getPort() const
            {
                return _port;
            }
            std::vector<sf::Event> getEvents() const
            {
                return clientEvents;
            }

            bool getIsServer() const
            {
                return isServer;
            }

            bool getGameHasStarted() const
            {
                return gameHasStarted;
            }

            std::map<int, std::vector<sf::Event>> &getServerEvents()
            {
                return serverEvents;
            }

            WaitingRoom &getWaitingRoom()
            {
                return waitingRoom;
            }

            std::vector<std::tuple<ECS::id_t, std::vector<ComponentType>,
                                   std::vector<std::pair<BaseComponent *, ComponentType>>>> &
            getComponentsToUpdate()
            {
                return componentsToUpdate;
            }

            ComponentsConvertor &getComponentsConvertor()
            {
                return componentsConvertor;
            }

            // =========================================================
            // ======================= SETTERS =========================
            // =========================================================

            void setIP(const sf::IpAddress &ip)
            {
                serverAddress = ip;
            }
            void setPort(int port)
            {
                _port = port;
            }
            // =========================================================
            // =================== EVENTS HANDLING =====================
            // =========================================================

            void addEvent(sf::Event event)
            {
                clientEvents.push_back(event);
            }

            void clearEvents()
            {
                clientEvents.clear();
            }

            void setIsReadyToStart(bool isReady)
            {
                isReadyToStart = isReady;
                sf::Packet packet;
                packet << static_cast<int>(PacketType::SwitchWorld);
                sendPacketToAllClients(packet);
            }

            void resetServer()
            {
                isServer       = false;
                gameHasStarted = false;
                isReadyToStart = false;
                serverAddress  = sf::IpAddress::None;
                serverHost     = std::make_pair(sf::IpAddress::None, 0);
                serverEvents.clear();
                componentsToUpdate.clear();
                waitingRoom.clear();
                stop();
            }

            bool getIsReadyToStart() const
            {
                return isReadyToStart;
            }

        private:
            // =========================================================
            // ================== GESTION DES CLIENTS ==================
            // =========================================================

            // Méthode pour gérer la connexion d'un client
            void handleClientConnection(const sf::IpAddress &sender, unsigned short senderPort, bool isHost = false)
            {
                sf::Packet responsePacket;
                responsePacket << static_cast<int>(PacketType::HandshakeResponse) << true;
                sendPacketToClient(responsePacket, sender, senderPort);
                addPlayerToLobby(sender, senderPort, isHost);
            }

            void addPlayerToLobby(const sf::IpAddress &clientAddress, unsigned short clientPort, bool isHost = false);

            void switchToGame();

            // =========================================================
            // ================== GESTION DES PAQUETS ==================
            // =========================================================

            void handleHandshakeRequest(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort);

            void handleLeaveLobby(sf::Packet &packet, const sf::IpAddress &sender);

            void handleClientIndependentInitialization(sf::Packet &packet);
            void handleInitializeGame(sf::Packet &packet, const sf::IpAddress &sender, unsigned short clientPort);
            void handleReceiveInitializedGame(const sf::IpAddress &sender, unsigned short clientPort);
            void handleSwitchWorld(const sf::IpAddress &sender, unsigned short clientPort);
            void handleReceiveSwitchedWorld(const sf::IpAddress &sender, unsigned short clientPort);

            void handleClientUpdate(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort);
            void handleKeyInputs(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort);

            int findPlayerNb(const sf::IpAddress &sender, unsigned short senderPort);

            // ================== ATTRIBUTS ==================

            WaitingRoom   waitingRoom;
            sf::UdpSocket socket;
            sf::IpAddress serverAddress = sf::IpAddress::None;

            std::thread thread;
            bool        running = false;
            int         _port   = 0;

            bool                                     isServer       = false;
            bool                                     gameHasStarted = false;
            bool                                     isReadyToStart = false;
            ComponentsConvertor                      componentsConvertor;
            std::vector<sf::Event>                   clientEvents;
            std::map<int, std::vector<sf::Event>>    serverEvents;
            std::pair<sf::IpAddress, unsigned short> serverHost;
            std::vector<std::tuple<ECS::id_t, std::vector<ComponentType>,
                                   std::vector<std::pair<BaseComponent *, ComponentType>>>>
                componentsToUpdate;
    };
} // namespace ECS
```

### **`ECS::Network` Class:**

- **Purpose**: Manages network communication for the ECS.
- **Attributes**:
    - **`waitingRoom`**: An instance of **`WaitingRoom`** to manage player connections.
    - **`socket`**: An instance of **`sf::UdpSocket`** for UDP communication.
    - **`serverAddress`**: An **`sf::IpAddress`** representing the server's IP address.
    - **`thread`**: A **`std::thread`** for asynchronous packet receiving.
    - **`running`**: A boolean indicating whether the network is running.
    - **`_port`**: An integer representing the port.
    - **`isServer`**: A boolean indicating whether the instance is a server.
    - **`gameHasStarted`**: A boolean indicating whether the game has started.
    - **`isReadyToStart`**: A boolean indicating whether the server is ready to start the game.
    - **`componentsConvertor`**: An instance of **`ComponentsConvertor`** for component conversion.
    - **`clientEvents`**: A vector of **`sf::Event`** representing client events.
    - **`serverEvents`**: A map of player numbers to vectors of **`sf::Event`** representing server events.
    - **`serverHost`**: A pair of **`sf::IpAddress`** and **`unsigned short`** representing the server host.
    - **`componentsToUpdate`**: A vector of tuples representing components to update.

    **Methods**:

### Server Handler:

- **`startServer(unsigned short port)`**: Start the server on the specified port.
- **`startClient(unsigned short port)`**: Start the client on the specified port.
- **`connectToServer(const sf::IpAddress &serverAddress, unsigned short port)`**: Connect to the server.
- **`sendConnectionToServer(const sf::IpAddress &serverAddress, unsigned short port)`**: Send connection information to the server.
- **`stop()`**: Stop the network.

### Send:

- **`sendUpdatedEntitiesToClients()`**: Send updated entities to clients.
- **`sendRemovedComponentsToClients()`**: Send removed components to clients.
- **`sendRemovedEntitiesToClients(std::vector<ECS::id_t> &removedEntities)`**: Send removed entities to clients.
- **`sendEventsToServer()`**: Send events to the server.
- **`getNbEntitiesModified()`**: Get the number of entities modified.
- **`getNbEntitiesWithDeletedComponents()`**: Get the number of entities with deleted components.
- **`addSerializedEventToPacket(sf::Packet &packet, sf::Event event)`**: Add a serialized event to the packet.
- **`addSerializedComponentToPacket(sf::Packet &packet, ECS::BaseComponent *component)`**: Add a serialized component to the packet.
- **`addSerializedEntityToPacket(sf::Packet &packet, const std::pair<const ECS::id_t, std::unique_ptr<ECS::Entity>> &pair)`**: Add a serialized entity to the packet.
- **`addSerializedDeletedEntityToPacket(sf::Packet &packet, const std::pair<const ECS::id_t, std::unique_ptr<ECS::Entity>> &pair)`**: Add a serialized deleted entity to the packet.
- **`sendPacketTypeToServer(PacketType packetType, const sf::IpAddress &recipient, unsigned short port)`**: Send the packet type to the server.
- **`sendPacketToServer(sf::Packet &packet)`**: Send a packet to the server.
- **`sendPacketToAllClients(sf::Packet &packet, bool includeServer = true)`**: Send a packet to all clients.
- **`sendPacketToClient(sf::Packet &packet, const sf::IpAddress &address, unsigned short port)`**: Send a packet to a specific client.

### Receive:

- **`receivePackets()`**: Receive packets.
- **`handleReceivedPacket(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort)`**: Handle received packets.
- **`deserializeEntityAndApply(sf::Packet &packet)`**: Deserialize entity information and apply.
- **`deserializeRemoveEntitiesAndRemove(sf::Packet &packet)`**: Deserialize information about removed entities and remove them.
- **`deserializeRemovedComponentsAndApply(sf::Packet &packet)`**: Deserialize information about removed components and apply.
- **`deserializeEvent(sf::Packet &packet)`**: Deserialize an event.

### Getters:

- **`getSocket()`**: Get the UDP socket.
- **`getIP()`**: Get the IP address.
- **`getPort()`**: Get the port.
- **`getEvents()`**: Get client events.
- **`getIsServer()`**: Check if the instance is a server.
- **`getGameHasStarted()`**: Check if the game has started.
- **`getServerEvents()`**: Get server events.
- **`getWaitingRoom()`**: Get the waiting room.
- **`getComponentsToUpdate()`**: Get components to update.
- **`getComponentsConvertor()`**: Get the components convertor.

### Setters:

- **`setIP(const sf::IpAddress &ip)`**: Set the IP address.
- **`setPort(int port)`**: Set the port.

### Events Handling:

- **`addEvent(sf::Event event)`**: Add an event.
- **`clearEvents()`**: Clear client events.
- **`setIsReadyToStart(bool isReady)`**: Set whether the server is ready to start.
- **`resetServer()`**: Reset the server attributes.
- **`getIsReadyToStart()`**: Check if the server is ready to start.

**Attributes**:

- **`WaitingRoom waitingRoom`**: An instance of the waiting room.
- **`sf::UdpSocket socket`**: The UDP socket used for communication.
- **`sf::IpAddress serverAddress`**: The server's IP address.
- **`std::thread thread`**: A thread for handling network operations.
- **`bool running`**: A flag indicating whether the network is running.
- **`int _port`**: The port used for communication.
- **`bool isServer`**: A flag indicating whether the instance is a server.
- **`bool gameHasStarted`**: A flag indicating whether the game has started.
- **`bool isReadyToStart`**: A flag indicating whether the server is ready to start.
- **`ComponentsConvertor componentsConvertor`**: An instance of the components convertor.
- **`std::vector<sf::Event> clientEvents`**: A vector to store client events.
- **`std::map<int, std::vector<sf::Event>> serverEvents`**: A map to store server events.
- **`std::pair<sf::IpAddress, unsigned short> serverHost`**: The server's IP address and port.
- **`std::vector<std::tuple<ECS::id_t, std::vector<ComponentType>, std::vector<std::pair<BaseComponent *, ComponentType>>>> componentsToUpdate`**: Vector to store components to update.

## WaitingRoom.hpp
```cpp
#pragma once

#include <SFML/Network.hpp>
#include <vector>

#include "Engine/Server/ClientInfo.hpp"

struct WaitingRoom {

        void addPlayer(const sf::IpAddress &player, unsigned short port, bool isServer = false)
        {
            players.push_back(new ClientInfo(player, port, ClientState::InLobby, isServer));
        }

        void clear()
        {
            players.clear();
            isStarted = false;
        }

        std::vector<ClientInfo *> getPlayers() const
        {
            return players;
        }

        bool isGameStarted() const
        {
            return isStarted;
        }

        bool isReadyToStart() const
        {
            return players.size() >= 2;
        }

        bool allPlayersSwitchedWorld() const
        {
            for (const auto &player : players) {
                if (!player->hasSwitchedWorld) {
                    return false;
                }
            }
            return true;
        }

        bool allPlayersInitializedGame()
        {
            for (const auto &player : players) {
                if (!player->isInitialized) {
                    return false;
                }
            }
            isStarted = true;
            return true;
        }

        void removePlayer(const sf::IpAddress &player)
        {
            for (auto it = players.begin(); it != players.end(); it++) {
                if ((*it)->address == player) {
                    players.erase(it);
                    return;
                }
            }
        }

        std::vector<ClientInfo *> getPlayers()
        {
            return players;
        }

    private:
        std::vector<ClientInfo *> players;
        bool                      isStarted = false;
};
```

### **`WaitingRoom` Structure:**

- **Purpose**: Represents a waiting room where players gather before the game starts.
- **Members**:
    - **`players`**: A vector of pointers to **`ClientInfo`** representing the players in the waiting room.
    - **`isStarted`**: A boolean indicating whether the game has started.
- **Member Functions**:
    - **`addPlayer`**: Adds a player to the waiting room with the given IP address, port, and server status.
    - **`clear`**: Clears the waiting room, resetting it to its initial state.
    - **`getPlayers`** (const): Returns a vector of pointers to **`ClientInfo`** representing the players in the waiting room.
    - **`isGameStarted`** (const): Returns a boolean indicating whether the game has started.
    - **`isReadyToStart`** (const): Returns a boolean indicating whether there are at least two players in the waiting room.
    - **`allPlayersSwitchedWorld`** (const): Returns a boolean indicating whether all players have switched to a different world.
    - **`allPlayersInitializedGame`**: Checks if all players in the waiting room have initialized the game. If true, sets **`isStarted`** to true.
    - **`removePlayer`**: Removes a player with the given IP address from the waiting room.