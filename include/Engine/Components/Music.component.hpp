/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Renderable.component.hpp
*/

#pragma once

#include <SFML/Audio.hpp>
#include <vector>

#include "ECS/Components.hpp"

namespace Engine::Components
{
    struct MusicComponent : public ECS::BaseComponent {
        public:
            MusicComponent(std::string path, float _volume = 50) : _volume(_volume)
            {
                music.openFromFile(path);
                music.setVolume(_volume);
                music.setLoop(true);
                music.play();
            }

            ~MusicComponent() override = default;

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

            float     _volume;
            sf::Music music;
    };
} // namespace Engine::Components
