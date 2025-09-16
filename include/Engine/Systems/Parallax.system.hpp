/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Parallax.system.hpp
*/

#pragma once

#include "ECS/System.hpp"
#include "ECS/World.hpp"

namespace Engine::System
{
    class ParallaxSystem : public ECS::BaseSystem
    {
        public:
            ParallaxSystem(ECS::World &world) : ECS::BaseSystem(world) {}

            void configure(ECS::World &world) override;

            void unconfigure() override;

            void tick() override;
    };
} // namespace Engine::System
