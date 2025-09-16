/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Animation.system.cpp
*/

#include "Engine/Systems/Animation.system.hpp"

#include "ECS/Components.hpp"
#include "ECS/World.hpp"
#include "Engine/Components/Animation.component.hpp"
#include "Engine/Components/LayeredAnimation.component.hpp"
#include "Engine/Components/LayeredRenderable.component.hpp"
#include "Engine/Components/Renderable.component.hpp"
#include "Engine/Engine.hpp"

using namespace Engine::System;

void AnimationSystem::configure([[maybe_unused]] ECS::World &world) {}

void AnimationSystem::unconfigure() {}

static void animateSprite(ECS::ComponentHandle<Engine::Components::RenderableComponent> &renderable,
                          ECS::ComponentHandle<Engine::Components::AnimationComponent>  &animation)
{
    if (animation->clock.getElapsedTime().asMilliseconds() > animation->animationSpeed) {
        animation->clock.restart();
        animation->frame++;
        if (animation->frame >= animation->frameCount) animation->frame = 0;
    }
    renderable->sprite.setTextureRect(
        sf::Rect<int>(animation->textureRect.left + animation->tileSize.x * animation->frame,
                      animation->textureRect.top, animation->textureRect.width, animation->textureRect.height));
}

void AnimationSystem::tick()
{
    ECS::World                &world    = getWorld();
    std::vector<ECS::Entity *> entities = world.getEntitiesWithComponents<Engine::Components::RenderableComponent,
                                                                          Engine::Components::AnimationComponent>();

    std::vector<ECS::Entity *> layeredEntities =
        world.getEntitiesWithComponents<Engine::Components::LayeredRenderableComponent,
                                        Engine::Components::LayeredAnimationComponent>();

    for (auto &entity : entities) {
        auto renderable = entity->getComponent<Engine::Components::RenderableComponent>();
        auto animation  = entity->getComponent<Engine::Components::AnimationComponent>();
        if (!renderable->isDisplayed || !animation->doAnimation) continue;
        animateSprite(renderable, animation);
    }
    for (auto &entity : layeredEntities) {
        auto renderables = entity->getComponent<Engine::Components::LayeredRenderableComponent>();
        if (!renderables->isDisplayed) continue;
        auto animations = entity->getComponent<Engine::Components::LayeredAnimationComponent>();

        for (int i = 0; i < animations->layers; i++) {
            if (!renderables->at(i)->isDisplayed || !animations->at(i)->doAnimation) continue;
            auto animation  = animations->at(i);
            auto renderable = renderables->at(i);
            animateSprite(renderable, animation);
        }
    }
}
