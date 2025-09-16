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
    struct SpeedComponent : public ECS::BaseComponent {
        public:
            SpeedComponent() : speed(0) {}
            SpeedComponent(float speed) : speed(speed) {}

            ~SpeedComponent() override = default;

            std::vector<char> serialize() override
            {
                std::ostringstream oss(std::ios::binary);
                oss.write(reinterpret_cast<const char *>(&speed), sizeof(speed));

                const std::string &str = oss.str();
                return {str.begin(), str.end()};
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) final
            {
                SpeedComponent *speedComponent;
                if (component == nullptr) {
                    speedComponent = new SpeedComponent(0.0f);
                } else {
                    speedComponent = dynamic_cast<SpeedComponent *>(component);
                    if (speedComponent == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                iss.read(reinterpret_cast<char *>(&speedComponent->speed), sizeof(speedComponent->speed));

                return speedComponent;
            }

            ComponentType getType() override
            {
                return ComponentType::SpeedComponent;
            }

            float speed;

        private:
    };
} // namespace Engine::Components
