/*
** EPITECH PROJECT, 2024
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Serialize
*/

#include "ECS/World.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Server/Network.hpp"

void ECS::Network::addSerializedEventToPacket(sf::Packet &packet, sf::Event event)
{
    packet << event.type;
    switch (event.type) {
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased:
            packet << static_cast<int>(event.key.code);
            packet << static_cast<int>(event.key.alt);
            packet << static_cast<int>(event.key.control);
            packet << static_cast<int>(event.key.shift);
            packet << static_cast<int>(event.key.system);
            break;
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
            packet << static_cast<int>(event.mouseButton.button);
            packet << static_cast<int>(event.mouseButton.x);
            packet << static_cast<int>(event.mouseButton.y);
            break;
        case sf::Event::MouseMoved:
            packet << static_cast<int>(event.type);
            packet << static_cast<int>(event.mouseMove.x);
            packet << static_cast<int>(event.mouseMove.y);
            break;
        case sf::Event::MouseWheelScrolled:
            packet << static_cast<int>(event.type);
            packet << static_cast<int>(event.mouseWheelScroll.wheel);
            packet << event.mouseWheelScroll.delta; // float
            packet << static_cast<int>(event.mouseWheelScroll.x);
            packet << static_cast<int>(event.mouseWheelScroll.y);
            break;
        default:
            break;
    }
}

void ECS::Network::addSerializedComponentToPacket(sf::Packet &packet, ECS::BaseComponent *component)
{
    if (component == nullptr) {
        std::cerr << "Component is null" << std::endl;
        return;
    }

    std::vector<char> serializedData = component->serialize();
    ComponentType     componentType  = component->getType();
    int               serializedSize = static_cast<int>(serializedData.size());

    packet << static_cast<int>(componentType);
    packet << serializedSize;
    if (serializedSize > 0) {
        for (char byte : serializedData) {
            packet << static_cast<sf::Uint8>(byte);
        }
    }
    component->setHasChanged(false);
}

void ECS::Network::addSerializedEntityToPacket(sf::Packet                                                     &packet,
                                               const std::pair<const ECS::id_t, std::unique_ptr<ECS::Entity>> &pair)
{
    int nbChangedComponents = 0;

    if (pair.second == nullptr) return;

    for (const auto &component : pair.second->getComponents()) {
        if (component.second->getType() != ComponentType::NoneComponent && component.second->hasChanged()) {
            nbChangedComponents++;
        }
    }
    if (nbChangedComponents == 0) return;

    packet << static_cast<sf::Uint64>(pair.first);
    packet << nbChangedComponents;
    for (const auto &component : pair.second->getComponents()) {
        if (component.second->hasChanged()) {
            if (component.second->getType() == ComponentType::NoneComponent) continue;
            addSerializedComponentToPacket(packet, component.second.get());
        }
    }
}

void ECS::Network::addSerializedDeletedEntityToPacket(
    sf::Packet &packet, const std::pair<const ECS::id_t, std::unique_ptr<ECS::Entity>> &pair)
{
    if (pair.second == nullptr) return;

    int nbCompsToDelete = pair.second->getComponentsToDelete().size();

    for (const auto &component : pair.second->getComponents()) {
        if (component.second->getType() != ComponentType::NoneComponent &&
            std::find(pair.second->getComponentsToDelete().begin(), pair.second->getComponentsToDelete().end(),
                      component.second->getType()) != pair.second->getComponentsToDelete().end()) {
            nbCompsToDelete--;
            pair.second->getComponentsToDelete().erase(std::find(pair.second->getComponentsToDelete().begin(),
                                                                 pair.second->getComponentsToDelete().end(),
                                                                 component.second->getType()));
        }
    }
    if (nbCompsToDelete <= 0) {
        pair.second->getComponentsToDelete().clear();
        return;
    }

    packet << static_cast<sf::Uint64>(pair.first);
    packet << nbCompsToDelete;
    for (const auto &component : pair.second->getComponentsToDelete())
        packet << static_cast<int>(component);
    pair.second->getComponentsToDelete().clear();
}