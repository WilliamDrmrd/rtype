/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Animation.system.cpp
*/

#include "Engine/Systems/UI.system.hpp"

#include <SFML/Window/Mouse.hpp>

#include "ECS/World.hpp"
#include "Engine/Engine.hpp"

using namespace Engine::System;

void UI::configure([[maybe_unused]] ECS::World &world) {}

void UI::unconfigure() {}

void UI::tick()
{
    using namespace Engine::Components;

    sf::RenderWindow *window        = &WINDOW;
    sf::Vector2i      mousePosition = sf::Mouse::getPosition(*window);
    sf::Vector2f      worldPos      = window->mapPixelToCoords(mousePosition);

    auto &world = getWorld();
    world.each<ButtonComponent, RenderableComponent>([&]([[maybe_unused]] ECS::Entity                    *entity,
                                                         const ECS::ComponentHandle<ButtonComponent>     &buttonComp,
                                                         const ECS::ComponentHandle<RenderableComponent> &renderable) {
        updateButtonState(buttonComp, renderable, worldPos);
    });
    world.each<CheckBoxComponent, RenderableComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, const ECS::ComponentHandle<CheckBoxComponent> &checkboxComp,
            const ECS::ComponentHandle<RenderableComponent> &renderable) {
            updateCheckBoxState(entity, checkboxComp, renderable, worldPos);
        });
    world.each<CursorComponent, PositionComponent, RenderableComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, const ECS::ComponentHandle<CursorComponent> &cursorComp,
            const ECS::ComponentHandle<PositionComponent>   &position,
            const ECS::ComponentHandle<RenderableComponent> &renderable) {
            updateCursorState(cursorComp, renderable, position, mousePosition, worldPos);
        });
    world.each<TextInputComponent, RenderableComponent>(
        [&]([[maybe_unused]] ECS::Entity *entity, const ECS::ComponentHandle<TextInputComponent> &textinputComp,
            const ECS::ComponentHandle<RenderableComponent> &renderable) {
            updateInputState(entity, textinputComp, renderable, worldPos);
        });
    world.each<TextComponent>([&]([[maybe_unused]] ECS::Entity                               *entity,
                                  [[maybe_unused]] const ECS::ComponentHandle<TextComponent> &textComp) {
        if (entity->getComponent<TextComponent>()->name == "nbPlayers")
            entity->getComponent<TextComponent>()->changeText(
                "Nombre de joueurs : " + std::to_string(NETWORK.getWaitingRoom().getPlayers().size()) + "/4");
    });
}

void UI::updateButtonState(ECS::ComponentHandle<Components::ButtonComponent>     buttonComp,
                           ECS::ComponentHandle<Components::RenderableComponent> renderable,
                           const sf::Vector2f                                   &worldPos)
{

    sf::FloatRect buttonRect(renderable->sprite.getGlobalBounds());

    resetButtonVisual(renderable);
    if (!buttonComp->isActivated) return;
    if (buttonRect.contains(worldPos)) {
        hoverEffect(buttonComp, renderable);
        handleClick(buttonComp, renderable);
    } else {
        buttonComp->isHovered = false;
        if (buttonComp->isClicked) {
            buttonComp->isClicked = false;
        }
    }
}

void UI::updateInputState(ECS::Entity *entity, ECS::ComponentHandle<Components::TextInputComponent> textInputComp,
                          ECS::ComponentHandle<Components::RenderableComponent> renderable,
                          const sf::Vector2f                                   &worldPos)
{

    sf::FloatRect buttonRect(renderable->sprite.getGlobalBounds());

    if (buttonRect.contains(worldPos)) {
        handleTextInput(entity, textInputComp, renderable);
    } else {
        if (textInputComp->isClicked) {
            textInputComp->isClicked = false;
        }
    }
}

void UI::updateCheckBoxState(ECS::Entity *entity, ECS::ComponentHandle<Components::CheckBoxComponent> checkboxComp,
                             ECS::ComponentHandle<Components::RenderableComponent> renderable,
                             const sf::Vector2f                                   &worldPos)
{
    sf::FloatRect buttonRect(renderable->sprite.getGlobalBounds());

    if (!checkboxComp->isActivated) return;
    if (buttonRect.contains(worldPos)) {
        handleCheck(entity, checkboxComp, renderable);
    } else {
        if (checkboxComp->isClicked) {
            checkboxComp->isClicked = false;
        }
    }
}

void UI::updateCursorState(ECS::ComponentHandle<Components::CursorComponent>          cursorComp,
                           ECS::ComponentHandle<Components::RenderableComponent>      renderable,
                           const ECS::ComponentHandle<Components::PositionComponent> &position,
                           sf::Vector2i mousePosition, const sf::Vector2f &worldPos)
{

    sf::FloatRect buttonRect(renderable->sprite.getGlobalBounds());

    resetButtonVisual(renderable);

    if (buttonRect.contains(worldPos))
        handleChange(cursorComp, position, mousePosition, renderable);
    else if (cursorComp->isClicked)
        cursorComp->isClicked = false;
}

void UI::resetButtonVisual(ECS::ComponentHandle<Components::RenderableComponent> renderable)
{
    renderable->sprite.setColor(sf::Color(255, 255, 255, 255));
    renderable->scale = renderable->savedScale;
}

void UI::hoverEffect(ECS::ComponentHandle<Components::ButtonComponent>     buttonComp,
                     ECS::ComponentHandle<Components::RenderableComponent> renderable)
{
    buttonComp->isHovered = true;
    renderable->sprite.setColor(sf::Color(255, 255, 255, 200));
}

void UI::handleQuitGame()
{
    Engine::EngineClass &engine = Engine::EngineClass::getEngine();
    engine.window.close();
}

void UI::handleStartGame()
{
    Engine::EngineClass &engine = Engine::EngineClass::getEngine();
    engine.switchWorld("game");
}

void UI::handleGoMenu()
{
    Engine::EngineClass &engine = Engine::EngineClass::getEngine();
    engine.switchWorld("menu");
}

void UI::handleGameOver(int playerNb)
{
    if (NETWORK.getIsServer()) {
        for (auto &player : NETWORK.getWaitingRoom().getPlayers()) {
            if (player->nbPlayer == playerNb) {
                player->isAlive = false;
            }
        }
        if (!NETWORK.getWaitingRoom().allPlayersAreDead()) return;
        NETWORK.sendGameOverToClients();
        Engine::EngineClass &engine = Engine::EngineClass::getEngine();
        engine.switchWorld("GameOver");
        NETWORK.resetServer();
    }
}

void UI::handleGoJoin()
{
    Engine::EngineClass &engine = Engine::EngineClass::getEngine();
    engine.switchWorld("joinRoom");
}

void UI::handleGoCreate()
{
    Engine::EngineClass &engine = Engine::EngineClass::getEngine();
    engine.switchWorld("createRoom");
}

void UI::handleGoWaitingHost()
{
    Engine::EngineClass &engine = Engine::EngineClass::getEngine();
    engine.switchWorld("waitingHost");
}

void UI::handleGoWaitingClient()
{
    Engine::EngineClass &engine = Engine::EngineClass::getEngine();
    engine.switchWorld("waitingClient");
}

void UI::handleClick(ECS::ComponentHandle<Components::ButtonComponent>     buttonComp,
                     ECS::ComponentHandle<Components::RenderableComponent> renderable)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        renderable->scale = {static_cast<float>(renderable->scale.x * 0.9),
                             static_cast<float>(renderable->scale.y * 0.9)};
        if (!buttonComp->isClicked) {
            buttonComp->isClicked = true;
        }
    } else if (buttonComp->isClicked) {
        buttonComp->onClick();
        buttonComp->isClicked = false;
    }
}

void UI::handleTextInput(ECS::Entity *entity, ECS::ComponentHandle<Components::TextInputComponent> textInputComp,
                         [[maybe_unused]] const ECS::ComponentHandle<Components::RenderableComponent> &renderable)
{
    using namespace Engine::Components;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!textInputComp->isClicked) {
            textInputComp->isClicked = true;
        }
    } else if (textInputComp->isClicked) {
        textInputComp->onChange();
        getWorld().each<TextInputComponent>(
            [&]([[maybe_unused]] ECS::Entity *entity, ECS::ComponentHandle<TextInputComponent> textinputComp) {
                textinputComp->isFocused = false;
            });
        textInputComp->isFocused = true;
        textInputComp->isClicked = false;
    }
}

void UI::handleCheck(ECS::Entity *entity, ECS::ComponentHandle<Components::CheckBoxComponent> checkboxComp,
                     const ECS::ComponentHandle<Components::RenderableComponent> &renderable)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!checkboxComp->isClicked) {
            checkboxComp->isClicked = true;
        }
    } else if (checkboxComp->isClicked) {
        checkboxComp->status = !checkboxComp->status;
        checkboxChangeRenderable(entity, checkboxComp, renderable);
        if (checkboxComp->status)
            checkboxComp->clickOn();
        else
            checkboxComp->clickOff();
        checkboxComp->isClicked = false;
    }
}

void UI::handleChange(ECS::ComponentHandle<Components::CursorComponent>   cursorComp,
                      ECS::ComponentHandle<Components::PositionComponent> position, sf::Vector2i mousePosition,
                      [[maybe_unused]] const ECS::ComponentHandle<Components::RenderableComponent> &renderable)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (position->x > 250 && position->x < 490) cursorComp->onChange();
        if (mousePosition.x > 250 && mousePosition.x < 490) {
            position->x = mousePosition.x;
        }
    }
}

void UI::checkboxChangeRenderable(
    ECS::Entity *entity, ECS::ComponentHandle<Components::CheckBoxComponent> checkboxComp,
    [[maybe_unused]] const ECS::ComponentHandle<Components::RenderableComponent> &renderable)
{

    if (checkboxComp->status) {
        entity->removeComponent<Components::RenderableComponent>();
        entity->addComponent<Components::RenderableComponent>(new Components::RenderableComponent(
            "./assets/menu/button_check/check_on.png", 0, 0, 3, 0, {2, 2}, false, true));
    } else {
        entity->removeComponent<Components::RenderableComponent>();
        entity->addComponent<Components::RenderableComponent>(new Components::RenderableComponent(
            "./assets/menu/button_check/check_off.png", 0, 0, 3, 0, {2, 2}, false, true));
    }
}

void UI::handleKeyboard() {}
