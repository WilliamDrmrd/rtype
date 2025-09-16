/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Clock.hpp
*/

#pragma once

#include <ctime>

#ifndef CLOCK_MIN_ELAPSED_TIME      // Minimum elapsed time between two calls to the update method of a system
#define CLOCK_MIN_ELAPSED_TIME 1000 // in milliseconds
#endif                              // !CLOCK_MIN_ELAPSED_TIME

namespace ECS
{
    class Clock
    {
        public:
            Clock()  = default;
            ~Clock() = default;

            std::clock_t operator==(const std::clock_t &other)
            {
                return std::clock() == other;
            }

            std::clock_t operator>=(const std::clock_t &other)
            {
                return std::clock() >= other;
            }

            std::clock_t operator<=(const std::clock_t &other)
            {
                return std::clock() <= other;
            }

            std::clock_t operator+(const std::clock_t &other)
            {
                return std::clock() + other;
            }

            std::clock_t operator-(const std::clock_t &other)
            {
                return std::clock() - other;
            }

            std::clock_t operator/(const std::clock_t &other)
            {
                return std::clock() / other;
            }

            std::clock_t getElapsedTime()
            {
                std::clock_t elapsed = std::clock() - _time;
                _time                = std::clock();
                return elapsed;
            }

            bool isValid()
            {
                return std::clock() >= CLOCK_MIN_ELAPSED_TIME;
            }

            void reset()
            {
                _time = 0;
            }

        private:
            std::clock_t _time;
    };
} // namespace ECS
