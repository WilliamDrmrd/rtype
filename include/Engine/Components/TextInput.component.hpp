/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** TextInput.component.hpp
*/

#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <utility>

#include "ECS/Components.hpp"

namespace Engine::Components
{
    struct TextInputComponent : public ECS::BaseComponent {
            std::string           text;
            std::function<void()> onChange;
            bool                  isFocused = false;
            bool                  isClicked = false;

            TextInputComponent(std::string defaultValue, std::function<void()> onChange)
                : text(std::move(defaultValue)), onChange(std::move(onChange))
            {
            }

            TextInputComponent()           = default;
            ~TextInputComponent() override = default;

            std::vector<char> serialize() override
            {
                std::ostringstream oss(std::ios::binary);

                const std::string &str = oss.str();
                return {str.begin(), str.end()};
            }

            ECS::BaseComponent *deserialize([[maybe_unused]] std::vector<char> vec,
                                            ECS::BaseComponent                *component) override
            {
                return component;
            }

            ComponentType getType() override
            {
                return ComponentType::NoneComponent;
            }
    };
} // namespace Engine::Components
