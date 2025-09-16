/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** ExcludeCollision.component.hpp
*/

#pragma once

#include <sstream>
#include <vector>

#include "ECS/Components.hpp"
#include "public/ComponentsType.hpp"

enum class ParallaxLayer {
    FarBackground,
    MidBackground,
    NearBackground
};

namespace Engine::Components
{
    struct ParallaxComponent : public ECS::BaseComponent {
            ParallaxLayer layer;
            float         speed;
            sf::Vector2f  offset;
            bool          first{};

            ParallaxComponent(ParallaxLayer layer, float speed) : layer(layer), speed(speed), offset(0, 0) {}

            ~ParallaxComponent() override = default;

            std::vector<char> serialize() override
            {
                std::ostringstream oss(std::ios::binary);
                auto               layerInt = static_cast<std::underlying_type<ParallaxLayer>::type>(layer);
                oss.write(reinterpret_cast<const char *>(&layerInt), sizeof(layerInt));
                oss.write(reinterpret_cast<const char *>(&speed), sizeof(speed));
                oss.write(reinterpret_cast<const char *>(&offset.x), sizeof(offset.x));
                oss.write(reinterpret_cast<const char *>(&offset.y), sizeof(offset.y));

                const std::string &str = oss.str();
                return std::vector<char>(str.begin(), str.end());
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) final
            {
                ParallaxComponent *parallaxComponent;
                if (component == nullptr) {
                    parallaxComponent = new ParallaxComponent(ParallaxLayer::FarBackground, 0);
                } else {
                    parallaxComponent = dynamic_cast<ParallaxComponent *>(component);
                    if (parallaxComponent == nullptr) return nullptr;
                }

                std::istringstream                        iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                std::underlying_type<ParallaxLayer>::type layerInt;
                iss.read(reinterpret_cast<char *>(&layerInt), sizeof(layerInt));
                parallaxComponent->layer = static_cast<ParallaxLayer>(layerInt);
                iss.read(reinterpret_cast<char *>(&parallaxComponent->speed), sizeof(parallaxComponent->speed));
                iss.read(reinterpret_cast<char *>(&parallaxComponent->offset.x), sizeof(parallaxComponent->offset.x));
                iss.read(reinterpret_cast<char *>(&parallaxComponent->offset.y), sizeof(parallaxComponent->offset.y));

                return parallaxComponent;
            }

            ComponentType getType() override
            {
                return ComponentType::NoneComponent;
            }
    };
} // namespace Engine::Components