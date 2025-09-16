/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** ServerHandler
*/

#include "Engine/Server/Network.hpp"

void ECS::Network::startServer(unsigned short port, bool isSolo)
{
    if (running) return;
    thread   = std::thread(&Network::startClient, this, port, isSolo);
    isServer = true;
    running  = true;
}

void ECS::Network::startClient(unsigned short port, bool isSolo)
{
    setIP("127.0.0.1");
    setPort(port);
    socket.setBlocking(false);
    if (isSolo) {
        while (socket.bind(port) != sf::Socket::Done) {
            port++;
        }
    } else {
        if (socket.bind(port) != sf::Socket::Done) {
            return;
        }
    }
    addPlayerToLobby("localhost", port, isServer);
    while (running) {
        receivePackets();
    }
    socket.unbind();
}

void ECS::Network::connectToServer(const sf::IpAddress &serverAddress, unsigned short port)
{
    if (running) return;
    setIP(serverAddress);
    setPort(port);
    thread   = std::thread(&Network::sendConnectionToServer, this, serverAddress, port);
    isServer = false;
    running  = true;
}

void ECS::Network::sendConnectionToServer(const sf::IpAddress &serverAddress, unsigned short port)
{
    sf::Packet packet;
    packet << static_cast<int>(PacketType::HandshakeRequest);
    packet << false;
    serverHost = std::make_pair(serverAddress, port);
    socket.setBlocking(false);
    if (socket.send(packet, serverAddress, port) != sf::Socket::Done) {
        std::cerr << "Erreur lors de l'envoi de la demande de connexion" << std::endl;
    }
    receivePackets();
}

void ECS::Network::stop()
{
    running = false;
    if (thread.joinable()) thread.join();
    if (socket.getLocalPort() != 0) socket.unbind();
}
