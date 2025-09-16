/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Resize.event.hpp
*/

#pragma once

#include <cstddef>

#include "ECS/Entity.hpp"

struct CollisionEvent {
        ECS::Entity *movingEntity;
        ECS::Entity *collidingEntity;
};
