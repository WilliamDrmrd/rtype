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
    class Sound : public ECS::BaseSystem
    {
        public:
            Sound(ECS::World &world) : ECS::BaseSystem(world){};
            Sound(ECS::World &world, ECS::id_t ids...) : ECS::BaseSystem(world, ids) {}
            ~Sound() override = default;

            void configure(ECS::World &world) override;

            void unconfigure() override;

            void tick() override;

            void UpVolumeMusic();

            void DownVolumeMusic();

            void UpVolumeSFX();

            void SetVolumeMusic();

            void DownVolumeSFX();

            void Mute();
    };
} // namespace Engine::System
