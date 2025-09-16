/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Renderable.component.hpp
*/

#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#include "ECS/Components.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "public/ComponentsType.hpp"

namespace Engine::Components
{
    struct WorldMoveProgressComponent : public ECS::BaseComponent {
        public:
            WorldMoveProgressComponent() : progress(0) {}
            WorldMoveProgressComponent(size_t startingTime, size_t progress, size_t speed)
                : startingTime(startingTime), progress(progress), speed(speed)
            {
            }

            ~WorldMoveProgressComponent() override = default;

            std::vector<char> serialize(void) override
            {
                std::ostringstream oss(std::ios::binary);
                oss.write(reinterpret_cast<const char *>(&startingTime), sizeof(startingTime));
                oss.write(reinterpret_cast<const char *>(&progress), sizeof(progress));
                oss.write(reinterpret_cast<const char *>(&speed), sizeof(speed));

                const std::string &str = oss.str();
                return std::vector<char>(str.begin(), str.end());
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) final
            {
                WorldMoveProgressComponent *worldMoveProgressComponent;
                if (component == nullptr) {
                    worldMoveProgressComponent = new WorldMoveProgressComponent(0, 0, 0);
                } else {
                    worldMoveProgressComponent = dynamic_cast<WorldMoveProgressComponent *>(component);
                    if (worldMoveProgressComponent == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                iss.read(reinterpret_cast<char *>(&worldMoveProgressComponent->startingTime),
                         sizeof(worldMoveProgressComponent->startingTime));
                iss.read(reinterpret_cast<char *>(&worldMoveProgressComponent->progress),
                         sizeof(worldMoveProgressComponent->progress));
                iss.read(reinterpret_cast<char *>(&worldMoveProgressComponent->speed),
                         sizeof(worldMoveProgressComponent->speed));

                return worldMoveProgressComponent;
            }

            ComponentType getType() override
            {
                return ComponentType::WorldMoveProgressComponent;
            }

            // in epoch milliseconds
            size_t startingTime;

            // this is the progress of the world in the x-axis,
            // the walls, enemies are spawning because of it.
            size_t progress;
            // speed in +x per second.
            size_t speed;

        private:
    };
} // namespace Engine::Components
