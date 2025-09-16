/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-ewen.sellitto
** File description:
** Renderable.component.hpp
*/

#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Vector2.hpp>
#include <sstream>
#include <vector>

#include "ECS/Components.hpp"

namespace Engine::Components
{
    struct ScoreComponent : public ECS::BaseComponent {
        public:
            ScoreComponent() = default;
            ScoreComponent(int score) : score(score) {}

            ~ScoreComponent() override = default;

            std::vector<char> serialize() override
            {
                std::ostringstream oss(std::ios::binary);

                oss.write(reinterpret_cast<const char *>(&score), sizeof(score));

                const std::string &str = oss.str();
                return {str.begin(), str.end()};
            }

            ECS::BaseComponent *deserialize(std::vector<char> vec, ECS::BaseComponent *component) final
            {
                ScoreComponent *scoreComponent;
                if (component == nullptr) {
                    scoreComponent = new ScoreComponent(0);
                } else {
                    scoreComponent = dynamic_cast<ScoreComponent *>(component);
                    if (scoreComponent == nullptr) return nullptr;
                }

                std::istringstream iss(std::string(vec.begin(), vec.end()), std::ios::binary);

                iss.read(reinterpret_cast<char *>(&scoreComponent->score), sizeof(scoreComponent->score));

                return scoreComponent;
            }

            ComponentType getType() override
            {
                return ComponentType::ScoreComponent;
            }

            int score = 0;

        private:
    };
} // namespace Engine::Components
