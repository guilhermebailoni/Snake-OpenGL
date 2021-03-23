#include "walls.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Walls::initializeGL(GLuint program) {
    terminateGL();

    m_program = program;
    m_colorLoc = glGetUniformLocation(m_program, "color");
    m_rotationLoc = glGetUniformLocation(m_program, "rotation");
    m_scaleLoc = glGetUniformLocation(m_program, "scale");
    m_translationLoc = glGetUniformLocation(m_program, "translation");
    m_pointSizeLoc = glGetUniformLocation(m_program, "pointSize");

    m_rotation = 0.0f;
    m_translation = glm::vec2(0);
    m_velocity = glm::vec2(0);

    m_innerWall = 45.0;
    float m_outerWall = 100.0;

    std::array<glm::vec2, 16> positions{
        //Top Border
        glm::vec2{-m_outerWall, +m_outerWall}, glm::vec2{-m_outerWall, +m_innerWall}, glm::vec2{+m_outerWall, +m_outerWall}, glm::vec2{+m_outerWall, +m_innerWall},
        //Right Border
        glm::vec2{+m_outerWall, -m_outerWall}, glm::vec2{+m_innerWall, -m_outerWall}, glm::vec2{+m_outerWall, +m_outerWall}, glm::vec2{+m_innerWall, +m_outerWall},
        //Left Border
        glm::vec2{-m_outerWall, -m_outerWall}, glm::vec2{-m_innerWall, -m_outerWall}, glm::vec2{-m_outerWall, +m_outerWall}, glm::vec2{-m_innerWall, +m_outerWall},
        //Bottom Border
        glm::vec2{-m_outerWall, -m_outerWall}, glm::vec2{-m_outerWall, -m_innerWall}, glm::vec2{+m_outerWall, -m_outerWall}, glm::vec2{+m_outerWall, -m_innerWall},
    };

    // Normalize
    for (auto &position :positions) {
        position /= glm::vec2{50.0f, 50.0f};
    }

    m_color = glm::vec4{0.2f,0.3f,0.5f,1.0f};
    // m_scale = 1.0f;

    // Generate VBO
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Get location of attributes in the program
    GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

    // Create VAO
    glGenVertexArrays(1, &m_vao);

    // Bind vertex attributes to current VAO
    glBindVertexArray(m_vao);

    glEnableVertexAttribArray(positionAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // End of binding to current VAO
    glBindVertexArray(0);
}

void Walls::paintGL() {

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  glUniform1f(m_rotationLoc, m_rotation);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);
  glUniform1f(m_pointSizeLoc, m_pointSize);

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  //glDrawArrays(GL_TRIANGLES, 0, 12);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
  glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
  glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

  glBindVertexArray(0);

  glUseProgram(0);
}

void Walls::terminateGL() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Walls::update(const Snake &snake, float deltaTime) {
    m_translation -= snake.m_velocity * deltaTime;
}