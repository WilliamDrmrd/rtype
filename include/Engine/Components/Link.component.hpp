/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Link.component.hpp
*/

#pragma once

#include <cstddef>

#include "ECS/Components.hpp"

namespace Engine::Components
{
    struct LinkComponent : ECS::BaseComponent {
        public:
            LinkComponent() : entity(0) {}
            LinkComponent(std::size_t id) : entity(id) {}
            ~LinkComponent() override = default;

            std::vector<char> serialize() override
            {
                std::ostringstream oss(std::ios::binary);
                oss.write(reinterpret_cast<const char *>(&entity), sizeof(entity));

                const std::string &str = oss.str();
                return {str.begin(), str.end()};
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) final
            {
                LinkComponent *linkComponent;
                if (component == nullptr) {
                    linkComponent = new LinkComponent(0);
                } else {
                    linkComponent = dynamic_cast<LinkComponent *>(component);
                    if (linkComponent == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                iss.read(reinterpret_cast<char *>(&linkComponent->entity), sizeof(linkComponent->entity));

                return linkComponent;
            }

            ComponentType getType() override
            {
                return ComponentType::LinkComponent;
            }
            std::size_t entity;
    };
} // namespace Engine::Components
