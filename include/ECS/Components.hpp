/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Components.hpp
*/

#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "public/ComponentsType.hpp"

namespace ECS
{

    /**
     * @class BaseComponent
     * @brief Abstract base class for components in the ECS (Entity-Component-System).
     * @brief You must provide a move constructor if one cannot be created implicitely
     */
    class BaseComponent
    {
        public:
            /**
             * @brief Virtual destructor for BaseComponent
             */
            virtual ~BaseComponent() = default;

            virtual std::vector<char>   serialize()                                                            = 0;
            virtual ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *comp = nullptr) = 0;

            virtual ComponentType getType() = 0;

            void setHasChanged(bool hasChanged)
            {
                _hasChanged = hasChanged;
            }

            bool hasChanged() const
            {
                return _hasChanged;
            }

        private:
            bool _hasChanged = true;
    };

    /**
     * @class ComponentHandle
     * @brief Template class to handle components.(when getting a component from an entity, you get a component handle)
     *
     * @tparam T The type of the component this handle is for.
     */
    template <typename T> class ComponentHandle
    {
        public:
            /**
             * @brief Default constructor initializing the component handle to nullptr.
             */
            ComponentHandle() : _component(nullptr){};

            /**
             * @brief Constructor initializing the component handle with a given component.
             *
             * @param component Pointer to the component.
             */
            explicit ComponentHandle(std::shared_ptr<T> component) : _component(component){};

            /**
             * @brief Default destructor.
             */
            ~ComponentHandle() = default;

            /*======================//
            //  Operator Overloads  //
            //======================*/

            /**
             * @brief Return the underlying component pointer.
             *
             * @return T* Pointer to the component.
             */
            T *operator()() const
            {
                return _component.get();
            }

            /**
             * @brief Overloading dereference operator to get the underlying component.
             *
             * @return T* Pointer to the component.
             */
            std::shared_ptr<T> operator*()
            {
                return get();
            }

            /**
             * @brief Overloading arrow operator to access the underlying component's members.
             *
             * @return T* Pointer to the component.
             */
            std::shared_ptr<T> operator->()
            {
                return get();
            }

            /**
             * @brief Deleted assignment operator to prevent copying of the component handle.
             */
            T *operator=(T *component) = delete;

            /**
             * @brief Compare this handle with another for equality.
             *
             * @param other The other ComponentHandle to compare with.
             * @return true If both handles point to the same component.
             * @return false Otherwise.
             */
            bool operator==(const ComponentHandle<T> &other)
            {
                return _component == other._component;
            }

            /**
             * @brief Compare this handle's validity with a boolean.
             *
             * @param other The boolean to compare with.
             * @return true If the handle's validity matches the boolean.
             * @return false Otherwise.
             */
            bool operator==(bool other)
            {
                return isValid() == other;
            }

            /**
             * @brief Compare this handle with another for inequality.
             *
             * @param other The other ComponentHandle to compare with.
             * @return true If both handles do not point to the same component.
             * @return false Otherwise.
             */
            bool operator!=(const ComponentHandle<T> &other)
            {
                return _component != other._component;
            }

            /**
             * @brief Compare this handle's validity with a boolean for inequality.
             *
             * @param other The boolean to compare with.
             * @return true If the handle's validity does not match the boolean.
             * @return false Otherwise.
             */
            bool operator!=(bool other)
            {
                return isValid() != other;
            }

            /*=====================//
            //  Validity Handling  //
            //=====================*/

            /**
             * @brief Check if the handle is valid (i.e., it points to a component).
             *
             * @return true If the handle is valid.
             * @return false Otherwise.
             */
            bool isValid()
            {
                return _component != nullptr;
            }

        private:
            std::shared_ptr<T> _component; ///< Pointer to the component.

            /**
             * @brief Get the underlying component pointer.
             *
             * @return T* Pointer to the component.
             */
            std::shared_ptr<T> get()
            {
                return _component;
            }
    };

} // namespace ECS
