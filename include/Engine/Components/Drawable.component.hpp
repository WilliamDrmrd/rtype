/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Drawable.component.hpp
*/

#pragma once

#include "ECS/Components.hpp"
#include "SFML/Graphics/Drawable.hpp"

class DrawableComponent : public ECS::BaseComponent
{
    public:
        DrawableComponent(sf::Drawable *drawable) : drawable(drawable){};

        ~DrawableComponent() override = default;

        std::vector<char> serialize() override
        {
            return {};
        }

        ECS::BaseComponent *deserialize([[maybe_unused]] std::vector<char> vec, ECS::BaseComponent *component) override
        {
            return component;
        }

        ComponentType getType() override
        {
            return ComponentType::NoneComponent;
        }

        sf::Drawable *drawable;
};
