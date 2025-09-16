/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Physics.systems.cpp
*/

#include "Engine/Systems/Physics.system.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "ECS/Components.hpp"
#include "ECS/Entity.hpp"
#include "Engine/Components/Collision.component.hpp"
#include "Engine/Components/ExcludeCollision.component.hpp"
#include "Engine/Components/Moving.component.hpp"
#include "Engine/Components/Position.component.hpp"
#include "Engine/Components/Renderable.component.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Events/Collision.event.hpp"

using namespace Engine::System;

void Physics::configure([[maybe_unused]] ECS::World &world) {}

void Physics::unconfigure() {}

sf::FloatRect Physics::get_hitbox_with_rotation(ECS::ComponentHandle<Engine::Components::CollisionComponent> &hitbox,
                                                int rotation, int x, int y)
{
    sf::FloatRect newHitbox = {};

    switch (static_cast<int>(rotation / 90)) {
        case 1:
            newHitbox = sf::FloatRect(x - hitbox->rect.top - hitbox->rect.width, y + hitbox->rect.left,
                                      hitbox->rect.height, hitbox->rect.width);
            break;
        case 2:
            newHitbox =
                sf::FloatRect(x - hitbox->rect.left - hitbox->rect.width, y - hitbox->rect.top - hitbox->rect.height,
                              hitbox->rect.width, hitbox->rect.height);
            break;
        case 3:
            newHitbox = sf::FloatRect(x + hitbox->rect.top, y - hitbox->rect.left - hitbox->rect.height,
                                      hitbox->rect.height, hitbox->rect.width);
            break;
        default:
            newHitbox =
                sf::FloatRect(x + hitbox->rect.left, y + hitbox->rect.top, hitbox->rect.width, hitbox->rect.height);
            break;
    }

    return newHitbox;
}

#ifndef NOT_SHOW_HITBOX
#include "Engine/Components/Drawable.component.hpp"
#endif

void Physics::collide(ECS::Entity *entity, int x, int y)
{
    using namespace Engine::Components;

    std::vector<ECS::Entity *> collisionEntities;
    ECS::World                &world = getWorld();

    if (!entity->has<CollisionComponent>()) return;

    ECS::ComponentHandle<CollisionComponent>        entity_col = entity->getComponent<CollisionComponent>();
    ECS::ComponentHandle<ExcludeCollisionComponent> entity_exclude_col;
    bool                                            hasExcludeCol = entity->has<ExcludeCollisionComponent>();

    if (hasExcludeCol) entity_exclude_col = entity->getComponent<ExcludeCollisionComponent>();

    sf::FloatRect hitbox;

    if (entity->has<RenderableComponent>()) {
        ECS::ComponentHandle<RenderableComponent> renderable = entity->getComponent<RenderableComponent>();
        hitbox = get_hitbox_with_rotation(entity_col, renderable->rotation, x, y);
    } else {
        hitbox = sf::FloatRect(+entity_col->rect.left, y + entity_col->rect.top, entity_col->rect.width,
                               entity_col->rect.height);
    }

    std::vector<ECS::Entity *> entities = world.getEntitiesWithComponents<CollisionComponent, PositionComponent>();

    for (auto ent : entities) {
        if (ent == entity) continue;
        if (hasExcludeCol && ent->has<ExcludeCollisionComponent>() &&
            entity_exclude_col->id == ent->getComponent<ExcludeCollisionComponent>()->id)
            continue;

        ECS::ComponentHandle<PositionComponent>  pos = ent->getComponent<PositionComponent>();
        ECS::ComponentHandle<CollisionComponent> col = ent->getComponent<CollisionComponent>();

        sf::FloatRect hit;

        if (ent->has<RenderableComponent>()) {
            ECS::ComponentHandle<RenderableComponent> renderable = ent->getComponent<RenderableComponent>();
            hit = get_hitbox_with_rotation(col, renderable->rotation, pos->x, pos->y);
        } else {
            hit = sf::FloatRect(pos->x + col->rect.left, pos->y + col->rect.top, col->rect.width, col->rect.height);
        }

#ifndef NOT_SHOW_HITBOX
        sf::RectangleShape *rect;
        if (!ent->has<DrawableComponent>()) {
            rect = new sf::RectangleShape(sf::Vector2f(hit.width, hit.height));
            ent->addComponent(new DrawableComponent(rect));
        } else {
            rect = (sf::RectangleShape *)ent->getComponent<DrawableComponent>()->drawable;
        }
        rect->setPosition(hit.left, hit.top);
        rect->setOutlineThickness(2);
        rect->setFillColor(sf::Color::Transparent);
        rect->setOutlineColor(sf::Color::Red);
#endif

        if (hitbox.intersects(hit)) collisionEntities.push_back(ent);
    }

    if (collisionEntities.empty()) return;
    for (auto &collisionEntity : collisionEntities)
        world.broadcastEvent<CollisionEvent>({entity, collisionEntity});
}

void Physics::moveTime(ECS::Entity *entity, ECS::ComponentHandle<Components::MovingComponent> handle)
{
    using namespace Engine::Components;

    auto   now         = std::chrono::high_resolution_clock::now();
    auto   epoch       = now.time_since_epoch();
    size_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();

    size_t       moveStartTime = handle->moveStartTime;
    size_t       moveDuration  = handle->moveDuration;
    sf::Vector2f initialPos    = handle->initialPos;
    sf::Vector2f moveAmount    = handle->moveAmount;

    size_t       elapsedTime = currentTime - moveStartTime;
    double       progress    = std::min(static_cast<double>(elapsedTime) / static_cast<double>(moveDuration), 1.0);
    sf::Vector2f newPosition = initialPos + moveAmount * static_cast<float>(progress);

    int endedMoveCounter = 0;

    if (moveAmount.x > 0 && newPosition.x >= initialPos.x + moveAmount.x) {
        newPosition.x = initialPos.x + moveAmount.x;
        endedMoveCounter++;
    }
    if (moveAmount.y > 0 && newPosition.y >= initialPos.y + moveAmount.y) {
        newPosition.y = initialPos.y + moveAmount.y;
        endedMoveCounter++;
    }
    if (moveAmount.x < 0 && newPosition.x <= initialPos.x + moveAmount.x) {
        newPosition.x = initialPos.x + moveAmount.x;
        endedMoveCounter++;
    }
    if (moveAmount.y < 0 && newPosition.y <= initialPos.y + moveAmount.y) {
        newPosition.y = initialPos.y + moveAmount.y;
        endedMoveCounter++;
    }
    if (!entity->has<PositionComponent>() || !entity->has<RenderableComponent>()) return;
    ECS::ComponentHandle<PositionComponent>   componentPos(entity->getComponent<PositionComponent>());
    ECS::ComponentHandle<RenderableComponent> renderableComponent(entity->getComponent<RenderableComponent>());

    collide(entity, newPosition.x, newPosition.y);

    if (!entity->has<MovingComponent>()) return;

    componentPos->x = static_cast<int>(newPosition.x);
    componentPos->y = static_cast<int>(newPosition.y);
    renderableComponent->sprite.setPosition(newPosition);

    if (endedMoveCounter == 2) entity->removeComponent<MovingComponent>(true);
}

void Physics::tick()
{
    using namespace Engine::Components;

    ECS::World &world = getWorld();

    world.each<MovingComponent>(
        [&](ECS::Entity *entity, ECS::ComponentHandle<MovingComponent> handle) { moveTime(entity, handle); });
}
