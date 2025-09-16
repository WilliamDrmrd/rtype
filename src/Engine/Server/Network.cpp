/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Engine.cpp
*/

#include "Engine/Server/Network.hpp"

#include "Engine/Engine.hpp"

using namespace Engine;
using namespace ECS;

void ECS::Network::switchToGame()
{
    using namespace Engine::Components;

    EngineClass &engine = EngineClass::getEngine();
    engine.switchWorld("game");
}
