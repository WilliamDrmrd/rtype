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
    struct ViewComponent : public ECS::BaseComponent {
        public:
            ViewComponent() : position(), view()
            {
                view.setCenter(sf::Vector2f(400, 300));
                view.setSize(sf::Vector2f(800, 600));
            }

            ViewComponent(sf::Vector2<float> position, sf::Vector2<float> size) : position(position), view()
            {
                view.setCenter(sf::Vector2f(position.x, position.y));
                view.setSize(sf::Vector2f(size.x, size.y));
            }

            ~ViewComponent() override = default;

            std::vector<char> serialize(void) override
            {
                std::ostringstream oss(std::ios::binary);
                oss.write(reinterpret_cast<const char *>(&position.x), sizeof(position.x));
                oss.write(reinterpret_cast<const char *>(&position.y), sizeof(position.y));

                const std::string &str = oss.str();
                return std::vector<char>(str.begin(), str.end());
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) final
            {
                ViewComponent *viewComponent;
                if (component == nullptr) {
                    viewComponent = new ViewComponent();
                } else {
                    viewComponent = dynamic_cast<ViewComponent *>(component);
                    if (viewComponent == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                iss.read(reinterpret_cast<char *>(&viewComponent->position.x), sizeof(viewComponent->position.x));
                iss.read(reinterpret_cast<char *>(&viewComponent->position.y), sizeof(viewComponent->position.y));

                // Configurer view en fonction de position
                viewComponent->view.setCenter(sf::Vector2f(viewComponent->position.x, viewComponent->position.y));
                // La taille de la vue doit être définie ici si nécessaire

                return viewComponent;
            }

            ComponentType getType() override
            {
                return ComponentType::ViewComponent;
            }

            sf::Vector2<float> position;
            sf::View           view;

        private:
    };
} // namespace Engine::Components
