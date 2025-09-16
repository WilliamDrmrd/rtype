/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** UISystem
*/

#pragma once

#include <chrono>

#include "ECS/System.hpp"
#include "ECS/World.hpp"

namespace Engine::System
{
    class WorldMoveSystem : public ECS::BaseSystem
    {
        public:
            WorldMoveSystem(ECS::World &world) : ECS::BaseSystem(world){};
            WorldMoveSystem(ECS::World &world, ECS::id_t ids...) : ECS::BaseSystem(world, ids) {}
            ~WorldMoveSystem() override = default;

            void configure(ECS::World &world) override;

            void unconfigure() override;

            void tick() override;

        private:
    };
} // namespace Engine::System
