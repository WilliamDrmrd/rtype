/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Text.component.hpp
*/

#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

#include "ECS/Components.hpp"
#include "SFML/Graphics/Text.hpp"

namespace Engine::Components
{
    struct TextComponent : public ECS::BaseComponent {
            sf::Text    text;
            std::string content;
            sf::Font    font;
            sf::Color   fillColor;
            bool        isDisplay;
            bool        centered;
            std::string name;

            TextComponent(const std::string &str, const sf::Font newFont, unsigned int characterSize, sf::Vector2f pos,
                          bool centered = false, bool isDisplay = true, std::string name = "")
                : isDisplay(isDisplay), centered(centered), name(name)

            {
                font = newFont;
                text.setString(str);
                text.setFont(font);
                text.setCharacterSize(characterSize);
                text.setPosition(pos);
                text.setFillColor(sf::Color::White);
                if (centered) {
                    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
                }
            }
            TextComponent(const std::string &str, const sf::Font newFont, unsigned int characterSize, sf::Vector2f pos,
                          const sf::Color &fillColor, bool centered = false, bool isDisplay = true,
                          std::string name = "")
                : isDisplay(isDisplay), centered(false), name(name)
            {
                font = newFont;
                text.setString(str);
                text.setFont(font);
                text.setCharacterSize(characterSize);
                text.setPosition(pos);
                text.setFillColor(fillColor);
                if (centered) {
                    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
                }
            }
            void changeText(const std::string &str)
            {
                content = str;
                text.setString(content);
            }

            void addText(const std::string &str)
            {
                content += str;
                text.setString(content);
            }

            void removeText()
            {
                if (content.size() > 0) content.pop_back();
                text.setString(content);
            }

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
