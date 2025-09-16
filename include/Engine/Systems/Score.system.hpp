/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** RenderEngine.hpp
*/

#pragma once

#include "ECS/System.hpp"
#include "ECS/World.hpp"

namespace Engine::System
{
    class ScoreSystem : public ECS::BaseSystem
    {
        public:
            ScoreSystem(ECS::World &world) : ECS::BaseSystem(world){};
            ScoreSystem(ECS::World &world, ECS::id_t ids...) : ECS::BaseSystem(world, ids) {}
            ~ScoreSystem() override = default;

            void configure(ECS::World &world) override;

            void unconfigure() override;

            void tick() override;

            void incrementScore();
    };
} // namespace Engine::System
