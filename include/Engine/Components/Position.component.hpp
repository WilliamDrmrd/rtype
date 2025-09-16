/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Position.component.hpp
*/

#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include "ECS/Components.hpp"
#include "public/ComponentsType.hpp"

namespace Engine::Components
{
    struct PositionComponent : public ECS::BaseComponent {
        public:
            PositionComponent() : x(0), y(0){};
            explicit PositionComponent(int x, int y) : x(x), y(y){};
            ~PositionComponent() override = default;

            std::vector<char> serialize() override
            {
                std::ostringstream oss(std::ios::binary);
                oss.write(reinterpret_cast<const char *>(&x), sizeof(x));
                oss.write(reinterpret_cast<const char *>(&y), sizeof(y));

                const std::string &str = oss.str();
                return {str.begin(), str.end()};
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) override
            {
                PositionComponent *positionComponent;
                if (component == nullptr) {
                    positionComponent = new PositionComponent();
                } else {
                    positionComponent = dynamic_cast<PositionComponent *>(component);
                    if (positionComponent == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                iss.read(reinterpret_cast<char *>(&positionComponent->x), sizeof(positionComponent->x));
                iss.read(reinterpret_cast<char *>(&positionComponent->y), sizeof(positionComponent->y));

                return positionComponent;
            }

            ComponentType getType() override
            {
                return ComponentType::PositionComponent;
            }

            int x;
            int y;
    };
} // namespace Engine::Components
