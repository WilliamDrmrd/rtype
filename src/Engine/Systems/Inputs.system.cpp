/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Inputs.system.cpp
*/

#include "Engine/Systems/Inputs.system.hpp"

#include <algorithm>

#include "ECS/World.hpp"
#include "Engine/Components/Text.component.hpp"
#include "Engine/Components/TextInput.component.hpp"
#include "Engine/Engine.hpp"

using namespace Engine::System;
using namespace Engine::Components;

void InputsSystem::configure([[maybe_unused]] ECS::World &world) {}

void InputsSystem::unconfigure() {}

void InputsSystem::tick() {}

void InputsSystem::changeText(std::string &text)
{
    ECS::World &world = getWorld();
    world.each<TextComponent, TextInputComponent>([&]([[maybe_unused]] ECS::Entity                        *entity,
                                                      [[maybe_unused]] ECS::ComponentHandle<TextComponent> textComp,
                                                      ECS::ComponentHandle<TextInputComponent> textInputComp) {
        if (textInputComp->isFocused && entity->has<TextComponent>()) {
            if (entity->getComponent<TextComponent>()->content.size() <= 15)
                entity->getComponent<TextComponent>()->addText(text);
        }
    });
}

void InputsSystem::removeText()
{
    ECS::World &world = getWorld();
    world.each<TextComponent, TextInputComponent>([&]([[maybe_unused]] ECS::Entity                        *entity,
                                                      [[maybe_unused]] ECS::ComponentHandle<TextComponent> textComp,
                                                      ECS::ComponentHandle<TextInputComponent> textInputComp) {
        if (textInputComp->isFocused && entity->has<TextComponent>()) {
            if (entity->getComponent<TextComponent>()->content.size() > 0)
                entity->getComponent<TextComponent>()->removeText();
        }
    });
}

bool InputsSystem::handleSend(std::vector<std::string> args)
{
    std::string ipAddress = "";
    int         port      = 0;
    WORLD.each<TextComponent, TextInputComponent>([&]([[maybe_unused]] ECS::Entity                        *entity,
                                                      [[maybe_unused]] ECS::ComponentHandle<TextComponent> textComp,
                                                      ECS::ComponentHandle<TextInputComponent> textInputComp) {
        std::string id = textInputComp->text;
        if (entity->has<TextComponent>()) {
            if (std::find(args.begin(), args.end(), id) != args.end()) {
                if (id == "IP") {
                    ipAddress = entity->getComponent<TextComponent>()->content;
                } else if (id == "PORT") {
                    if (entity->getComponent<TextComponent>()->content.empty()) {
                        port = 0;
                    } else {
                        port = std::stoi(entity->getComponent<TextComponent>()->content);
                    }
                }
            }
        }
    });

    if (args.size() == 2) {
        if (ipAddress == "" || port == 0) {
            return false;
        }
        NETWORK.connectToServer(ipAddress, port);
    } else {
        if (port == 0) {
            return false;
        }
        NETWORK.startServer(port, false);
    }
    return true;
}
