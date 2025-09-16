/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** main.cpp
*/

#include <SFML/Window/Event.hpp>
#include <thread>

#include "Engine/Engine.hpp"
#include "R-Type/GameWorld/GameWorld.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv, [[maybe_unused]] char **env)
{
    Engine::EngineClass &engine = Engine::EngineClass::getEngine();

#ifndef DEBUG_MODE
    try {
#endif
        engine.setStartWorld("menu");
        engine.addWorldFactory("menu", GameWorld::createMenuWorld);
        engine.addWorldFactory("createRoom", GameWorld::createRoomWorld);
        engine.addWorldFactory("game", GameWorld::createGameWorld);
        engine.addWorldFactory("GameOver", GameWorld::createGameOver);
        engine.addWorldFactory("joinRoom", GameWorld::joinRoomWorld);
        engine.addWorldFactory("waitingHost", GameWorld::createWaitingHost);
        engine.addWorldFactory("waitingClient", GameWorld::createWaitingClient);

        engine.run();
#ifndef DEBUG_MODE
    } catch (std::exception &e) {
        return 84;
    }
#endif

    engine.network().stop();

    return 0;
}
