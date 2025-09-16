/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** RenderEngine.hpp
*/

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

            void startServer(unsigned short port, bool isSolo);
            void startClient(unsigned short port, bool isSolo);

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

            void sendGameOverToClients();
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

            bool getNeedToReset() const
            {
                return needToReset;
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
                needToReset    = false;
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

            void handleEndGame(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort);
            void handleClientUpdate(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort);
            void handleKeyInputs(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort);

            int findPlayerNb(const sf::IpAddress &sender, unsigned short senderPort);

            // ================== ATTRIBUTS ==================

            WaitingRoom   waitingRoom;
            sf::UdpSocket socket;
            sf::IpAddress serverAddress = sf::IpAddress::None;

            std::thread thread;
            bool        running = false;
            bool        needToReset = false;
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
