/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** EventSubscriber.hpp
*/

#pragma once

#include <string>

#include "ECS/Components.hpp"
#include "ECS/Entity.hpp"

namespace ECS
{

    /**
     * @class BaseEventSubscriber
     * @brief Abstract base class for event subscribers in an ECS framework.
     *
     * This class provides the interface for any event subscriber that can
     * listen to and respond to events within the ECS.
     */
    class BaseEventSubscriber
    {
        public:
            /**
             * @brief Virtual destructor for BaseEventSubscriber, defined as pure virtual (0).
             */
            virtual ~BaseEventSubscriber() = default;
    };

    /**
     * @class EventSubscriber
     * @brief Template class for event subscribers specialized to certain event types.
     *
     * @tparam T The type of the event data that this subscriber is interested in.
     */
    template <typename T> class EventSubscriber : public BaseEventSubscriber
    {
        public:
            /**
             * @brief Virtual destructor for EventSubscriber, defined as pure virtual (0).
             */
            ~EventSubscriber() override = default;

            /**
             * @brief Receive and process an event.
             *
             * @param name The name of the event.
             * @param data A ref to the event data, expected to be of type T.
             *
             * @warning This method can cause infinite recursion if its broadcasting an event of type T or
             * it broadcast an event of type T and another subscriber is also subscribed to that event.
             */
            virtual void receiveEvent(const std::string &name, const T &data) = 0;
    };

    namespace Events
    {
        struct OnEntityCreated {
                Entity *entity;
        };

        struct OnEntityDestroyed {
                Entity *entity;
        };

        template <typename T> struct OnComponentAdded {
                Entity             *entity;
                ComponentHandle<T> *component;
        };

        template <typename T> struct OnComponentRemoved {
                Entity             *entity;
                ComponentHandle<T> *component;
        };
    } // namespace Events
} // namespace ECS
