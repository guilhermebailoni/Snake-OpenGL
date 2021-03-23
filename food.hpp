#ifndef FOOD_HPP_
#define FOOD_HPP_

#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "snake.hpp"
#include "walls.hpp"

class OpenGLWindow;

class Food {
    public:
        void initializeGL(GLuint program, glm::vec2 m_oldTranslation);
        void paintGL();
        void terminateGL();

        void update(const Snake &snake, float deltaTime);

    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLint m_colorLoc{};
        GLint m_rotationLoc{};
        GLint m_translationLoc{};
        GLint m_scaleLoc{};
        GLint m_pointSizeLoc{};

        GLuint m_vao{};
        GLuint m_vbo{};

        glm::vec4 m_color{1};
        bool m_hit{false};
        float m_rotation{};
        float m_scale{1.0f};
        float m_pointSize{};
        glm::vec2 m_translation{glm::vec2(0)};
        glm::vec2 m_oldTranslation;
        glm::vec2 m_velocity{glm::vec2(0)};

        std::default_random_engine m_randomEngine;
        std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
};

#endif