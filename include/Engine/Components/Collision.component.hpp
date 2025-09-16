/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Renderable.component.hpp
*/

#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "ECS/Components.hpp"
#include "R-Type/sprites.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "public/ComponentsType.hpp"

namespace Engine::Components
{
    struct CollisionComponent : public ECS::BaseComponent {
        public:
            explicit CollisionComponent(float off_x = 0, float off_y = 0, float size_x = 0, float size_y = 0)
                : rect(off_x, off_y, size_x, size_y)
            {
            }
            explicit CollisionComponent(sf::FloatRect rect) : rect(rect) {}

            explicit CollisionComponent(const std::string &spriteName) : name(spriteName)
            {
                auto it = spriteInfoMap.find(spriteName);
                if (it == spriteInfoMap.end()) throw std::runtime_error("Cannot find sprite " + spriteName);
                const SpriteInfo &info = it->second;
                rect                   = sf::FloatRect(info.offsetX, info.offsetY, info.width, info.height);
            }
            ~CollisionComponent() override = default;

            std::vector<char> serialize() override
            {
                std::ostringstream oss(std::ios::binary);
                oss.write(reinterpret_cast<const char *>(&rect.left), sizeof(rect.left));
                oss.write(reinterpret_cast<const char *>(&rect.top), sizeof(rect.top));
                oss.write(reinterpret_cast<const char *>(&rect.width), sizeof(rect.width));
                oss.write(reinterpret_cast<const char *>(&rect.height), sizeof(rect.height));

                const std::string &str = oss.str();
                return {str.begin(), str.end()};
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) final
            {
                CollisionComponent *collisionComponent;
                if (component == nullptr) {
                    collisionComponent = new CollisionComponent();
                } else {
                    collisionComponent = dynamic_cast<CollisionComponent *>(component);
                    if (collisionComponent == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                iss.read(reinterpret_cast<char *>(&collisionComponent->rect.left),
                         sizeof(collisionComponent->rect.left));
                iss.read(reinterpret_cast<char *>(&collisionComponent->rect.top), sizeof(collisionComponent->rect.top));
                iss.read(reinterpret_cast<char *>(&collisionComponent->rect.width),
                         sizeof(collisionComponent->rect.width));
                iss.read(reinterpret_cast<char *>(&collisionComponent->rect.height),
                         sizeof(collisionComponent->rect.height));

                return collisionComponent;
            }

            ComponentType getType() override
            {
                return ComponentType::CollisionComponent;
            }

            sf::FloatRect rect;
            std::string   name;
    };
} // namespace Engine::Components
