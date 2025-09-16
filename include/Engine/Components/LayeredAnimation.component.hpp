/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** LayeredAnimation.component.hpp
*/

#pragma once

#include <vector>

#include "ECS/Components.hpp"
#include "Engine/Components/Animation.component.hpp"

namespace Engine::Components
{
    class LayeredAnimationComponent : public ECS::BaseComponent
    {
        public:
            LayeredAnimationComponent()           = default;
            ~LayeredAnimationComponent() override = default;

            /**
             * @brief Construct a new Layered Animation Component object
             *
             * @param first
             * @param rest
             * @note the order of the animations is the order of the parameters and is linked with the layered render
             * component order
             */
            template <typename... Animations>
            explicit LayeredAnimationComponent(AnimationComponent *first, Animations... rest)
                : animation{std::shared_ptr<AnimationComponent>(first), std::shared_ptr<AnimationComponent>(rest)...},
                  layers(0)
            {
                layers = animation.size();
            }

            std::vector<char> serialize() override
            {
                unsigned long      size = animation.size();
                std::ostringstream oss(std::ios::binary);

                oss.write(reinterpret_cast<const char *>(&layers), sizeof(layers));
                oss.write(reinterpret_cast<const char *>(&size), sizeof(size));
                for (auto &anim : animation) {
                    std::vector<char> animSerialized = anim->serialize();

                    size_t animSize = animSerialized.size();
                    oss.write(reinterpret_cast<const char *>(&animSize), sizeof(animSize));
                    oss.write(reinterpret_cast<const char *>(animSerialized.data()), animSize);
                }

                const std::string &str = oss.str();
                return {str.begin(), str.end()};
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) override
            {
                unsigned long              size;
                LayeredAnimationComponent *layered_animation_component;
                if (component == nullptr) {
                    layered_animation_component = new LayeredAnimationComponent();
                } else {
                    layered_animation_component = dynamic_cast<LayeredAnimationComponent *>(component);
                    if (layered_animation_component == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                iss.read(reinterpret_cast<char *>(&layered_animation_component->layers),
                         sizeof(layered_animation_component->layers));
                iss.read(reinterpret_cast<char *>(&size), sizeof(size));
                for (unsigned long i = 0; i < size; i++) {
                    size_t animSize;
                    iss.read(reinterpret_cast<char *>(&animSize), sizeof(animSize));
                    std::vector<char> animSerialized(animSize);
                    iss.read(reinterpret_cast<char *>(animSerialized.data()), animSize);
                    auto *anim = new AnimationComponent();
                    anim       = dynamic_cast<AnimationComponent *>(anim->deserialize(animSerialized, anim));
                    layered_animation_component->animation.push_back(std::shared_ptr<AnimationComponent>(anim));
                }

                return layered_animation_component;
            }

            ECS::ComponentHandle<AnimationComponent> at(int index)
            {
                return ECS::ComponentHandle<AnimationComponent>(animation[index]);
            }

            ComponentType getType() override
            {
                return ComponentType::LayeredAnimationComponent;
            }

            std::vector<std::shared_ptr<AnimationComponent>> animation;
            int                                              layers{};
    };

} // namespace Engine::Components
