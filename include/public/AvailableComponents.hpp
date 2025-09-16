/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** RenderEngine.hpp
*/

#pragma once

#include <functional>

#include "ECS/Entity.hpp"
#include "Engine/Components/Animation.component.hpp"
#include "Engine/Components/Collision.component.hpp"
#include "Engine/Components/ExcludeCollision.component.hpp"
#include "Engine/Components/LayeredAnimation.component.hpp"
#include "Engine/Components/LayeredRenderable.component.hpp"
#include "Engine/Components/Link.component.hpp"
#include "Engine/Components/Moving.component.hpp"
#include "Engine/Components/Position.component.hpp"
#include "Engine/Components/Renderable.component.hpp"
#include "Engine/Components/Score.component.hpp"
#include "Engine/Components/Speed.component.hpp"
#include "Engine/Components/View.component.hpp"
#include "Engine/Components/WorldMoveProgress.component.hpp"
#include "R-Type/Components/BaseBullet.component.hpp"
#include "R-Type/Components/Booster.component.hpp"
#include "R-Type/Components/BoosterActive.component.hpp"
#include "R-Type/Components/Enemy.component.hpp"
#include "R-Type/Components/EnemyAttack.component.hpp"
#include "R-Type/Components/EnemyMovements.component.hpp"
#include "R-Type/Components/EnemyQueue.component.hpp"
#include "R-Type/Components/Health.component.hpp"
#include "R-Type/Components/Missile.component.hpp"
#include "R-Type/Components/Player.component.hpp"
#include "R-Type/Components/WeaponType.component.hpp"
#include "public/ComponentsType.hpp"

class ComponentsConvertor
{
    public:
        ComponentsConvertor()
        {
            // add here your components.
            registerComponent<Engine::Components::AnimationComponent>(ComponentType::AnimationComponent);
            registerComponent<Engine::Components::CollisionComponent>(ComponentType::CollisionComponent);
            registerComponent<Engine::Components::ExcludeCollisionComponent>(ComponentType::ExcludeCollisionComponent);
            registerComponent<Engine::Components::MovingComponent>(ComponentType::MovingComponent);
            registerComponent<Engine::Components::PositionComponent>(ComponentType::PositionComponent);
            registerComponent<Engine::Components::RenderableComponent>(ComponentType::RenderableComponent);
            registerComponent<Engine::Components::SpeedComponent>(ComponentType::SpeedComponent);
            registerComponent<Engine::Components::ViewComponent>(ComponentType::ViewComponent);
            registerComponent<Engine::Components::WorldMoveProgressComponent>(
                ComponentType::WorldMoveProgressComponent);
            registerComponent<Engine::Components::BaseBulletComponent>(ComponentType::BaseBulletComponent);
            registerComponent<Engine::Components::EnemyComponent>(ComponentType::EnemyComponent);
            registerComponent<Engine::Components::EnemyAttackComponent>(ComponentType::EnemyAttackComponent);
            registerComponent<Engine::Components::EnemyMovementsComponent>(ComponentType::EnemyMovementsComponent);
            registerComponent<Engine::Components::EnemyQueueComponent>(ComponentType::EnemyQueueComponent);
            registerComponent<Engine::Components::MissileComponent>(ComponentType::MissileComponent);
            registerComponent<Engine::Components::PlayerComponent>(ComponentType::PlayerComponent);
            registerComponent<Engine::Components::LinkComponent>(ComponentType::LinkComponent);
            registerComponent<Rtype::Components::HealthComponent>(ComponentType::HealthComponent);
            registerComponent<Engine::Components::LayeredRenderableComponent>(
                ComponentType::LayeredRenderableComponent);
            registerComponent<Engine::Components::LayeredAnimationComponent>(ComponentType::LayeredAnimationComponent);
            registerComponent<Rtype::Components::WeaponTypeComponent>(ComponentType::WeaponTypeComponent);
            registerComponent<Engine::Components::ScoreComponent>(ComponentType::ScoreComponent);
            registerComponent<Rtype::Components::BoosterComponent>(ComponentType::BoosterComponent);
            registerComponent<Rtype::Components::isBoosterComponent>(ComponentType::isBoosterComponent);
            registerComponent<Rtype::Components::BoosterActiveComponent>(ComponentType::BoosterActiveComponent);
        }

        template <typename T> void registerComponent(ComponentType type)
        {
            components[type] = []() -> ECS::BaseComponent * { return new T(); };

            destroyers[type] = [](ECS::Entity &entity) -> bool {
                if (entity.has<T>()) {
                    entity.removeComponent<T>();
                    return true;
                }
                return false;
            };

            adders[type] = [](ECS::Entity &entity, ECS::BaseComponent *comp) -> bool {
                if (entity.has<T>()) return false;
                if (dynamic_cast<T *>(comp) == nullptr) return false;
                entity.addComponent<T>(dynamic_cast<T *>(comp));
                return true;
            };
        }

        ECS::BaseComponent *createComponent(ComponentType type)
        {
            auto it = components.find(type);
            if (it != components.end()) return it->second();
            return nullptr;
        }

        std::map<ComponentType, std::function<ECS::BaseComponent *()>>                              components;
        std::map<ComponentType, std::function<bool(ECS::Entity &)>>                                 destroyers;
        std::map<ComponentType, std::function<bool(ECS::Entity &entity, ECS::BaseComponent *comp)>> adders;
};
