/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** LayeredRenderable.component.hpp
*/

#pragma once

#include <vector>

#include "ECS/Components.hpp"
#include "Engine/Components/Renderable.component.hpp"

namespace Engine::Components
{
    struct LayeredRenderableComponent : ECS::BaseComponent {

            LayeredRenderableComponent() = default;
            LayeredRenderableComponent(int priority, RenderableComponent *first)
                : renderable{std::shared_ptr<RenderableComponent>(first)}, layers(0), priority(priority)
            {
                layers = renderable.size();
            }
            template <typename... Renderables>
            LayeredRenderableComponent(int priority, RenderableComponent *first, Renderables... rest)
                : renderable{std::shared_ptr<RenderableComponent>(first),
                             std::shared_ptr<RenderableComponent>(rest)...},
                  layers(0), priority(priority)
            {
                layers = renderable.size();
            }
            ~LayeredRenderableComponent() override = default;

            std::vector<char> serialize() override
            {
                unsigned long size = renderable.size();

                std::ostringstream oss(std::ios::binary);
                oss.write(reinterpret_cast<const char *>(&priority), sizeof(priority));
                oss.write(reinterpret_cast<const char *>(&layers), sizeof(layers));
                oss.write(reinterpret_cast<const char *>(&isDisplayed), sizeof(isDisplayed));
                oss.write(reinterpret_cast<const char *>(&size), sizeof(size));

                for (auto &renderableComp : renderable) {
                    std::vector<char> renderableSerialized = renderableComp->serialize();
                    size_t            renderableSize       = renderableSerialized.size();
                    oss.write(reinterpret_cast<const char *>(&renderableSize), sizeof(renderableSize));
                    oss.write(reinterpret_cast<const char *>(renderableSerialized.data()), renderableSize);
                }

                const std::string &str = oss.str();
                return std::vector<char>(str.begin(), str.end());
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) override
            {
                unsigned long               size;
                LayeredRenderableComponent *layeredAnimationComponent;
                if (component == nullptr) {
                    layeredAnimationComponent = new LayeredRenderableComponent();
                } else {
                    layeredAnimationComponent = dynamic_cast<LayeredRenderableComponent *>(component);
                    if (layeredAnimationComponent == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);
                iss.read(reinterpret_cast<char *>(&layeredAnimationComponent->priority),
                         sizeof(layeredAnimationComponent->priority));
                iss.read(reinterpret_cast<char *>(&layeredAnimationComponent->layers),
                         sizeof(layeredAnimationComponent->layers));
                iss.read(reinterpret_cast<char *>(&layeredAnimationComponent->isDisplayed),
                         sizeof(layeredAnimationComponent->isDisplayed));
                iss.read(reinterpret_cast<char *>(&size), sizeof(size));

                for (unsigned long i = 0; i < size; i++) {
                    size_t animSize;
                    iss.read(reinterpret_cast<char *>(&animSize), sizeof(animSize));
                    std::vector<char> animSerialized(animSize);
                    iss.read(reinterpret_cast<char *>(animSerialized.data()), animSize);
                    auto *renderableComponent = new RenderableComponent();
                    renderableComponent->deserialize(animSerialized, renderableComponent);
                    renderableComponent->setTexture();
                    layeredAnimationComponent->renderable.push_back(
                        std::shared_ptr<RenderableComponent>(renderableComponent));
                }

                return layeredAnimationComponent;
            }

            ECS::ComponentHandle<RenderableComponent> at(int index)
            {
                return ECS::ComponentHandle<RenderableComponent>(renderable[index]);
            }

            ComponentType getType() override
            {
                return ComponentType::LayeredRenderableComponent;
            }

            std::vector<std::shared_ptr<RenderableComponent>> renderable;
            int                                               layers{};
            int                                               priority{};
            bool                                              isDisplayed = true;
    };

} // namespace Engine::Components
