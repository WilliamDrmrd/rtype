/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** System.hpp
*/

#pragma once

#include <vector>

#include "Clock.hpp"
#include "Utilities.hpp"

namespace ECS
{
    class World;

    /**
     * @class BaseSystem
     * @brief Abstract base class for systems in an ECS framework.
     *
     * @note This class provides the interface and common functionality for systems,
     * which are responsible for processing entities that have a specific set of components.
     */
    class BaseSystem
    {
        public:
            /**
             * @brief Construct a new BaseSystem object.
             *
             * @param world Reference to the World object that this system is part of.
             */
            explicit BaseSystem(World &world) : _world(world), _entities_id(), _clock(){};

            /**
             * @brief Construct a new BaseSystem object and initialize it with a list of entity IDs.
             *
             * @param world Reference to the World object that this system is part of.
             * @param ids A variable number of entity IDs to be processed by this system.
             */
            BaseSystem(World &world, id_t ids...) : _world(world), _entities_id()
            {
                _entities_id.emplace_back(ids);
            }

            /**
             * @brief Virtual destructor for the BaseSystem.
             */
            virtual ~BaseSystem() = default;

            /**
             * @brief Configure the system with the world.
             *
             * @note This method is intended to be implemented by derived classes to set up any necessary
             * state or resources related to the world.
             *
             * @param world Reference to the World object.
             */
            virtual void configure(World &world) = 0;

            /**
             * @brief Unconfigure the system, releasing any resources or state.
             *
             * @note This method is intended to be implemented by derived classes to clean up when the system is no
             * longer needed.
             */
            [[maybe_unused]] virtual void unconfigure() = 0;

            /**
             * @brief Update method for the system to process its entities.
             *
             * @note This method should be implemented by derived classes to define the system's behavior each tick.
             */
            virtual void tick() = 0;

            /**
             * @brief Get the World object that this system is part of.
             *
             * @return Reference to the World object.
             */
            [[nodiscard]] World &getWorld() const
            {
                return _world;
            }

        private:
            [[maybe_unused]] World &_world;       ///< Reference to the World this system belongs to.
            std::vector<id_t>       _entities_id; ///< List of entity IDs that this system processes.
            [[maybe_unused]] Clock  _clock{};     ///< Clock used to track the time between ticks.
    };

    class HigherOrderBaseSystem : public BaseSystem
    {
    };
} // namespace ECS
