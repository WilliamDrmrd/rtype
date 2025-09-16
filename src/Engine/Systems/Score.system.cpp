/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Animation.system.cpp
*/

#include "Engine/Systems/Score.system.hpp"

#include <SFML/Window/Mouse.hpp>

#include "ECS/World.hpp"
#include "Engine/Components/Score.component.hpp"
#include "Engine/Components/Text.component.hpp"
#include "Engine/Engine.hpp"

using namespace Engine::System;

void ScoreSystem::configure([[maybe_unused]] ECS::World &world) {}

void ScoreSystem::unconfigure() {}

void ScoreSystem::tick()
{
    WORLD.each<Engine::Components::ScoreComponent, Engine::Components::TextComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<Engine::Components::ScoreComponent> score,
            ECS::ComponentHandle<Engine::Components::TextComponent> ScoreText) {
            ScoreText->text.setString("score : " + std::to_string(score->score));
        });
}

void ScoreSystem::incrementScore()
{
    ECS::World &world = getWorld();
    world.each<Engine::Components::ScoreComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<Engine::Components::ScoreComponent> score) {
            score->score += 10;
        });
}
