/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Renderable.component.hpp
*/

#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
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
    struct MovingComponent : public ECS::BaseComponent {
        public:
            MovingComponent() : moveStartTime(0), initialPos({0, 0}), moveAmount({0, 0}), moveDuration(0){};
            MovingComponent(sf::Vector2f initialPos, size_t moveDuration, sf::Vector2f moveAmount)
                : initialPos(initialPos), moveAmount(moveAmount), moveDuration(moveDuration)
            {
                if (moveDuration == 0) this->moveDuration = 1;
                auto now      = std::chrono::high_resolution_clock::now();
                auto epoch    = now.time_since_epoch();
                moveStartTime = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
            }

            ~MovingComponent() override = default;

            std::vector<char> serialize() override
            {
                std::ostringstream oss(std::ios::binary);
                oss.write(reinterpret_cast<const char *>(&initialPos.x), sizeof(initialPos.x));
                oss.write(reinterpret_cast<const char *>(&initialPos.y), sizeof(initialPos.y));
                oss.write(reinterpret_cast<const char *>(&moveAmount.x), sizeof(moveAmount.x));
                oss.write(reinterpret_cast<const char *>(&moveAmount.y), sizeof(moveAmount.y));
                oss.write(reinterpret_cast<const char *>(&moveDuration), sizeof(moveDuration));
                oss.write(reinterpret_cast<const char *>(&moveStartTime), sizeof(moveStartTime));

                const std::string &str = oss.str();
                return {str.begin(), str.end()};
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) final
            {
                MovingComponent *movingComponent;
                if (component == nullptr) {
                    movingComponent = new MovingComponent({0, 0}, 0, {0, 0});
                } else {
                    movingComponent = dynamic_cast<MovingComponent *>(component);
                    if (movingComponent == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                iss.read(reinterpret_cast<char *>(&movingComponent->initialPos.x),
                         sizeof(movingComponent->initialPos.x));
                iss.read(reinterpret_cast<char *>(&movingComponent->initialPos.y),
                         sizeof(movingComponent->initialPos.y));
                iss.read(reinterpret_cast<char *>(&movingComponent->moveAmount.x),
                         sizeof(movingComponent->moveAmount.x));
                iss.read(reinterpret_cast<char *>(&movingComponent->moveAmount.y),
                         sizeof(movingComponent->moveAmount.y));
                iss.read(reinterpret_cast<char *>(&movingComponent->moveDuration),
                         sizeof(movingComponent->moveDuration));
                iss.read(reinterpret_cast<char *>(&movingComponent->moveStartTime),
                         sizeof(movingComponent->moveStartTime));

                return movingComponent;
            }

            ComponentType getType() override
            {
                return ComponentType::MovingComponent;
            }

            size_t       moveStartTime{};
            sf::Vector2f initialPos;
            sf::Vector2f moveAmount;
            size_t       moveDuration;

        private:
    };
} // namespace Engine::Components
