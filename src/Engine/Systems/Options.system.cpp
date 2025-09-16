/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Options.system.cpp
*/

#include "Engine/Systems/Options.system.hpp"

#include "ECS/World.hpp"
#include "Engine/Components/Button.component.hpp"
#include "Engine/Components/CheckBox.component.hpp"
#include "Engine/Components/Menu.component.hpp"
#include "Engine/Components/Options.component.hpp"
#include "Engine/Components/Renderable.component.hpp"
#include "Engine/Components/Text.component.hpp"

using namespace Engine::System;
using namespace Engine::Components;

void OptionsSystem::configure([[maybe_unused]] ECS::World &world) {}

void OptionsSystem::unconfigure() {}

void OptionsSystem::tick() {}

void OptionsSystem::openOptions()
{
    ECS::World &world = getWorld();
    world.each<RenderableComponent, MenuComponent>([&]([[maybe_unused]] ECS::Entity                        *entity,
                                                       ECS::ComponentHandle<RenderableComponent>            renderable,
                                                       [[maybe_unused]] ECS::ComponentHandle<MenuComponent> menuComp) {
        if (entity->has<ButtonComponent>()) entity->getComponent<ButtonComponent>()->isActivated = false;
        if (entity->has<CheckBoxComponent>()) entity->getComponent<CheckBoxComponent>()->isActivated = false;
        renderable->isDisplayed = false;
    });
    world.each<TextComponent, MenuComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<TextComponent> textComp,
            [[maybe_unused]] ECS::ComponentHandle<MenuComponent> menuComp) { textComp->isDisplay = false; });
    world.each<OptionsComponent, RenderableComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, [[maybe_unused]] ECS::ComponentHandle<OptionsComponent> options,
            ECS::ComponentHandle<RenderableComponent> renderable) {
            if (entity->has<ButtonComponent>()) entity->getComponent<ButtonComponent>()->isActivated = true;
            if (entity->has<CheckBoxComponent>()) entity->getComponent<CheckBoxComponent>()->isActivated = true;
            renderable->isDisplayed = true;
        });
    world.each<TextComponent, OptionsComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<TextComponent> textComp,
            [[maybe_unused]] ECS::ComponentHandle<OptionsComponent> options) { textComp->isDisplay = true; });
}

void OptionsSystem::closeOptions()
{
    ECS::World &world = getWorld();
    world.each<OptionsComponent, RenderableComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, [[maybe_unused]] ECS::ComponentHandle<OptionsComponent> options,
            ECS::ComponentHandle<RenderableComponent> renderable) {
            if (entity->has<ButtonComponent>()) entity->getComponent<ButtonComponent>()->isActivated = false;
            if (entity->has<CheckBoxComponent>()) entity->getComponent<CheckBoxComponent>()->isActivated = false;
            renderable->isDisplayed = false;
        });
    world.each<TextComponent, OptionsComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<TextComponent> textComp,
            [[maybe_unused]] ECS::ComponentHandle<OptionsComponent> options) { textComp->isDisplay = false; });
    world.each<RenderableComponent, MenuComponent>([&]([[maybe_unused]] ECS::Entity                        *entity,
                                                       ECS::ComponentHandle<RenderableComponent>            renderable,
                                                       [[maybe_unused]] ECS::ComponentHandle<MenuComponent> menuComp) {
        if (entity->has<ButtonComponent>()) entity->getComponent<ButtonComponent>()->isActivated = true;
        if (entity->has<CheckBoxComponent>()) entity->getComponent<CheckBoxComponent>()->isActivated = true;
        renderable->isDisplayed = true;
    });
    world.each<TextComponent, MenuComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<TextComponent> textComp,
            [[maybe_unused]] ECS::ComponentHandle<MenuComponent> menuComp) { textComp->isDisplay = true; });
}
