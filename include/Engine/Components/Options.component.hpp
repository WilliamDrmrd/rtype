/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Options.component.hpp
*/

#pragma once

#include <vector>

#include "ECS/Components.hpp"

namespace Engine::Components
{
    struct OptionsComponent : public ECS::BaseComponent {

        public:
            OptionsComponent() {}

            std::vector<char> serialize() override
            {
                return {};
            }

            ECS::BaseComponent *deserialize([[maybe_unused]] const std::vector<char> vec,
                                            [[maybe_unused]] ECS::BaseComponent     *component) final
            {
                return component;
            }

            ComponentType getType() override
            {
                return ComponentType::NoneComponent;
            }
    };
} // namespace Engine::Components
