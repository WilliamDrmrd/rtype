/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Parallax.system.cpp
*/

#include "Engine/Systems/Parallax.system.hpp"

#include "ECS/World.hpp"
#include "Engine/Components/Parallax.component.hpp"
#include "Engine/Components/Position.component.hpp"
#include "Engine/Components/Renderable.component.hpp"
#include "Engine/Components/View.component.hpp"

using namespace Engine::System;
using namespace Engine::Components;

void ParallaxSystem::configure([[maybe_unused]] ECS::World &world) {}

void ParallaxSystem::unconfigure() {}

void ParallaxSystem::tick()
{
    ECS::World  &world      = getWorld();
    sf::Vector2f windowSize = {0, 0};
    sf::Vector2f viewCenter = {0, 0};

    world.each<ViewComponent>(
        [&windowSize, &viewCenter]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<ViewComponent> viewComp) {
            windowSize = viewComp->view.getSize();
            viewCenter = viewComp->view.getCenter();
        });

    world.each<ParallaxComponent, PositionComponent,
               RenderableComponent>([&]([[maybe_unused]] ECS::Entity             *entity,
                                        ECS::ComponentHandle<ParallaxComponent>   parallaxComp,
                                        ECS::ComponentHandle<PositionComponent>   positionComp,
                                        ECS::ComponentHandle<RenderableComponent> renderable) {
        sf::Vector2f renderableSize = {static_cast<float>(renderable->size.x), static_cast<float>(renderable->size.y)};

        if (positionComp->x + renderableSize.x <= 0) {
            positionComp->x = renderableSize.x;
        }
        positionComp->x -= parallaxComp->speed;
    });
}
