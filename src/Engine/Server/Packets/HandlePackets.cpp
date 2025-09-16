/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** HandlePackets
*/

#include <typeindex>

#include "ECS/World.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Server/Network.hpp"
#include "R-Type/GameWorld/GameWorld.hpp"

void ECS::Network::handleEndGame(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort)
{
    needToReset = true;
}

void ECS::Network::handleHandshakeRequest(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort)
{
    bool isHost = false;
    packet >> isHost;

    if (waitingRoom.getPlayers().size() >= 4) {
        return;
    }
    handleClientConnection(sender, senderPort, isHost);
}

void ECS::Network::handleLeaveLobby([[maybe_unused]] sf::Packet &packet, const sf::IpAddress &sender)
{
    waitingRoom.removePlayer(sender);
}

void ECS::Network::handleInitializeGame(sf::Packet &packet, const sf::IpAddress &sender, unsigned short clientPort)
{
    using namespace Engine::Components;
    int nbEntities = 0;
    int nbPlayers  = 0;

    packet >> nbPlayers;
    Engine::EngineClass::getEngine().setPlayersAmount(nbPlayers);

    packet >> nbEntities;
    for (int i = 0; i < nbEntities; i++) {
        deserializeEntityAndApply(packet);
    }
    sf::Packet responsePacket;
    responsePacket << static_cast<int>(PacketType::InitializeGameOkForMe);
    sendPacketToClient(responsePacket, sender, clientPort);
}

void ECS::Network::handleReceiveInitializedGame(const sf::IpAddress &sender, unsigned short clientPort)
{
    for (const auto &player : waitingRoom.getPlayers()) {
        if (player->address == sender && player->port == clientPort) {
            player->isInitialized = true;
            break;
        }
    }

    if (waitingRoom.allPlayersInitializedGame()) {
        sf::Packet packet;
        packet << static_cast<int>(PacketType::LaunchGame);
        sendPacketToAllClients(packet);
    }
}

void ECS::Network::handleClientIndependentInitialization(sf::Packet &packet)
{
    int ownPlayerNb = 0;
    packet >> ownPlayerNb;

    Engine::EngineClass::getEngine().setOwnPlayer(ownPlayerNb);
    Engine::EngineClass::getEngine().setCurrentPlayer(ownPlayerNb);
}

void ECS::Network::handleSwitchWorld(const sf::IpAddress &sender, unsigned short senderPort)
{
    if ((NETWORK.getIsServer() && NETWORK.getWaitingRoom().allPlayersSwitchedWorld()) || !NETWORK.getIsServer())
        switchToGame();
    sf::Packet responsePacket;
    responsePacket << static_cast<int>(PacketType::SwitchWorldOkForMe);
    sendPacketToClient(responsePacket, sender, senderPort);
}

int ECS::Network::getNbEntitiesModified()
{
    int nbEntities = 0;
    for (const auto &pair : Engine::EngineClass::getEngine().world().getEntities()) {
        for (const auto &component : pair.second->getComponents()) {
            if (component.second->getType() != ComponentType::NoneComponent && component.second->hasChanged()) {
                nbEntities++;
                break;
            }
        }
    }

    return nbEntities;
}

int ECS::Network::getNbEntitiesWithDeletedComponents()
{
    int nbEntities      = 0;
    int nbCompsToDelete = 0;

    for (const auto &pair : Engine::EngineClass::getEngine().world().getEntities()) {
        nbCompsToDelete = static_cast<int>(pair.second->getComponentsToDelete().size());
        for (const auto &component : pair.second->getComponents()) {
            if (component.second->getType() != ComponentType::NoneComponent &&
                std::find(pair.second->getComponentsToDelete().begin(), pair.second->getComponentsToDelete().end(),
                          component.second->getType()) != pair.second->getComponentsToDelete().end()) {
                nbCompsToDelete--;
            }
        }
        if (nbCompsToDelete > 0) nbEntities++;
    }

    return nbEntities;
}

void ECS::Network::handleReceiveSwitchedWorld(const sf::IpAddress &sender, unsigned short clientPort)
{
    for (const auto &player : waitingRoom.getPlayers()) {
        if (player->address == sender && player->port == clientPort) {
            player->hasSwitchedWorld = true;
            break;
        }
    }

    int playerToSend = 0;
    for (const auto &player : waitingRoom.getPlayers()) {
        sf::Packet clientInitialization;
        clientInitialization << static_cast<int>(PacketType::ClientIndependentInitialization);
        clientInitialization << playerToSend;
        sendPacketToClient(clientInitialization, player->address, player->port);
        player->nbPlayer = playerToSend;
        playerToSend++;
    }

    if (waitingRoom.allPlayersSwitchedWorld()) {

        sf::Packet packet;
        packet << static_cast<int>(PacketType::InitializeGame);
        packet << static_cast<int>(waitingRoom.getPlayers().size());
        switchToGame();
        int nbEntities = getNbEntitiesModified();
        packet << nbEntities;
        for (const auto &pair : WORLD.getEntities())
            addSerializedEntityToPacket(packet, pair);
        sendPacketToAllClients(packet);
    }
}

void ECS::Network::addPlayerToLobby(const sf::IpAddress &clientAddress, unsigned short clientPort, bool isHost)
{
    waitingRoom.addPlayer(clientAddress, clientPort, isHost);
}

void ECS::Network::handleClientUpdate(sf::Packet &packet, [[maybe_unused]] const sf::IpAddress &sender,
                                      [[maybe_unused]] unsigned short senderPort)
{
    int nbEntities = 0;
    int updateType = 0;

    packet >> updateType;
    packet >> nbEntities;
    for (int i = 0; i < nbEntities; i++) {
        if (updateType == static_cast<int>(UpdateType::AddComponents)) {
            deserializeEntityAndApply(packet);
        } else if (updateType == static_cast<int>(UpdateType::RemoveComponents)) {
            deserializeRemovedComponentsAndApply(packet);
        } else if (updateType == static_cast<int>(UpdateType::RemoveEntity)) {
            deserializeRemoveEntitiesAndRemove(packet);
        }
    }
}

int ECS::Network::findPlayerNb(const sf::IpAddress &sender, unsigned short senderPort)
{
    for (const auto &player : waitingRoom.getPlayers()) {
        if (player->address == sender && player->port == senderPort) {
            return player->nbPlayer;
        }
    }
    return -1;
}

void ECS::Network::handleKeyInputs(sf::Packet &packet, const sf::IpAddress &sender, unsigned short senderPort)
{
    int nbEvents = 0;
    packet >> nbEvents;
    for (int i = 0; i < nbEvents; i++) {
        sf::Event event    = deserializeEvent(packet);
        int       playerNb = findPlayerNb(sender, senderPort);
        if (playerNb == -1) continue;
        serverEvents[playerNb].push_back(event);
    }
}
