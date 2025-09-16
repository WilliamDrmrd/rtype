/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** ClientInfo.hpp
*/

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
        bool           isAlive          = true;

        ClientInfo() : port(0), state(ClientState::Disconnected) {} // Constructeur par défaut

        ClientInfo(const sf::IpAddress &addr, unsigned short prt, ClientState st, bool isServer)
            : address(addr), port(prt), state(st), isServer(isServer)
        {
        }
};
