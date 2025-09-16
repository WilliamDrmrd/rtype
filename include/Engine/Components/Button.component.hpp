/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Renderable.component.hpp
*/

#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "ECS/Components.hpp"
#include "public/ComponentsType.hpp"

enum class ButtonType {
    Simple
};

namespace Engine::Components
{
    struct ButtonComponent : public ECS::BaseComponent {
            std::string           text;
            std::function<void()> onClick;
            bool                  isActivated;
            bool                  isHovered = false;
            bool                  isClicked = false;

            ButtonComponent(const std::string &text, std::function<void()> onClick, bool isActivated = true)
                : text(text), onClick(onClick), isActivated(isActivated)
            {
            }

            ~ButtonComponent() override = default;

            std::vector<char> serialize() override
            {
                return {};
            }

            ECS::BaseComponent *deserialize([[maybe_unused]] const std::vector<char> vec,
                                            [[maybe_unused]] ECS::BaseComponent     *component) final
            {
                return nullptr;
            }

            ComponentType getType() override
            {
                return ComponentType::NoneComponent;
            }
    };
} // namespace Engine::Components
