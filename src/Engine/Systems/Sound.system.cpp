/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Animation.system.cpp
*/

#include "Engine/Systems/Sound.system.hpp"

#include <SFML/Window/Mouse.hpp>

#include "ECS/World.hpp"
#include "Engine/Components/Music.component.hpp"
#include "Engine/Engine.hpp"

using namespace Engine::System;

void Sound::configure([[maybe_unused]] ECS::World &world) {}

void Sound::unconfigure() {}

void Sound::tick() {}

void Sound::UpVolumeMusic()
{
    ECS::World &world = getWorld();
    world.each<Engine::Components::MusicComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<Engine::Components::MusicComponent> music) {
            if (music->music.getVolume() < 100) {
                music->music.setVolume(music->music.getVolume() + 10);
            }
        });
}

void Sound::DownVolumeMusic()
{
    ECS::World &world = getWorld();
    world.each<Engine::Components::MusicComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<Engine::Components::MusicComponent> music) {
            if (music->music.getVolume() > 10) {
                music->music.setVolume(music->music.getVolume() - 10);
            } else if (music->music.getVolume() > 0) {
                music->music.setVolume(0);
            }
        });
}

void Sound::SetVolumeMusic()
{
    ECS::World       &world          = getWorld();
    sf::RenderWindow *window         = &WINDOW;
    sf::Vector2i      mousePosition  = sf::Mouse::getPosition(*window);
    float             cursorPosition = mousePosition.x - 250;
    float             volume         = (cursorPosition / 240) * 100;
    world.each<Engine::Components::MusicComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<Engine::Components::MusicComponent> music) {
            if (volume > 100)
                volume = 100;
            else if (volume < 1)
                volume = 0;
            music->music.setVolume(volume);
        });
}
void Sound::UpVolumeSFX() {}
void Sound::DownVolumeSFX() {}
void Sound::Mute() {}