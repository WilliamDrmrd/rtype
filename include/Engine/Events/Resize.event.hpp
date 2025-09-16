/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Resize.event.hpp
*/

#pragma once

#include <cstddef>

struct ResizeEvent {
        ResizeEvent(std::size_t x, std::size_t y) : x(x), y(y) {}

        std::size_t x;
        std::size_t y;
};
