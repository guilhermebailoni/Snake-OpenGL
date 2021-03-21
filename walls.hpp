#ifndef WALLS_HPP_
#define WALLS_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"
#include "snake.hpp"

class OpenGLWindow;

class Walls {
    public:
        void initializeGL(GLuint program);
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
        glm::vec2 m_velocity{glm::vec2(0)};
};

#endif