/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Animation.system.cpp
*/

#include "Engine/Systems/WorldMove.system.hpp"

#include <SFML/Window/Mouse.hpp>

#include "ECS/World.hpp"
#include "Engine/Components/WorldMoveProgress.component.hpp"
#include "Engine/Engine.hpp"

using namespace Engine::System;

void WorldMoveSystem::configure([[maybe_unused]] ECS::World &world) {}

void WorldMoveSystem::unconfigure() {}

void WorldMoveSystem::tick()
{
    using namespace Engine::Components;

    ECS::World &world = getWorld();

    ECS::Entity *worldMoveEntity = world.getEntityWithComponents<WorldMoveProgressComponent>();

    if (!worldMoveEntity) {
        return;
    }

    ECS::ComponentHandle<WorldMoveProgressComponent> handle =
        worldMoveEntity->getComponent<WorldMoveProgressComponent>();

    auto   now         = std::chrono::high_resolution_clock::now();
    auto   epoch       = now.time_since_epoch();
    size_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();

    handle->progress = static_cast<size_t>(((currentTime - handle->startingTime) * 50) / 1000);
}
