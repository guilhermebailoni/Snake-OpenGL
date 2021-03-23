#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;
class Walls;
class Food;

class Snake {
    public:
        void initializeGL(GLuint program);
        void paintGL(const GameData &gameData);
        void terminateGL();

        void update(const GameData &gameData, float deltaTime);
        void setRotation(float rotation) { m_rotation = rotation; }

    private:
        friend OpenGLWindow;
        friend Walls;
        friend Food;
        
        GLuint m_program{};
        GLint m_translationLoc{};
        GLint m_colorLoc{};
        GLint m_scaleLoc{};
        GLint m_rotationLoc{};
        GLint m_pointSizeLoc{};

        GLuint m_vao{};
        GLuint m_vbo{};

        glm::vec4 m_color{1};
        float m_rotation{};
        float m_scale{1.0f};
        float m_pointSize{};
        glm::vec2 m_translation{glm::vec2(0)};
        glm::vec2 m_velocity{glm::vec2(0)};
};

#endif