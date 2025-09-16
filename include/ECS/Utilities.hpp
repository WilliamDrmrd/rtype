/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Utilities.hpp
*/

#pragma once

#include <cstddef>

#define ECS_TYPEID(T) typeid(T).hash_code()

namespace ECS
{
    typedef std::size_t id_t;
    typedef std::size_t type_t;

    /**
     * @class Utils
     * @brief Utility class for the ECS.
     */
    class Utils
    {
        public:
            /**
             * @brief Get a new unique identifier.
             *
             * @tparam T The type of the identifier.
             * @return id_t A new unique identifier for this type.
             * @note Typedefs will have the same identifier as their underlying type.
             */
            template <typename T> static id_t getNewId(id_t idNew = 9999999999)
            {
                static id_t id = 0;

                if (idNew < id) return idNew;
                // static std::unordered_map<T, id_t> ids();
                if (idNew < 9999999999) {
                    id = idNew + 1;
                    return idNew;
                }
                return id++;
            }
    };
} // namespace ECS
