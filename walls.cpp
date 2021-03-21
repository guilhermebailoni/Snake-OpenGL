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
    m_pointSize = 50.0f;

    std::array<glm::vec2, 12> positions{
        //Top Border
        glm::vec2{-55.0f, +45.0f}, glm::vec2{+0.0f, +60.0f}, glm::vec2{+55.0f, +45.0f},
        //Right Border
        glm::vec2{+45.0f, +55.0f}, glm::vec2{+60.0f, +0.0f}, glm::vec2{+45.0f, -55.0f},
        //Left Border
        glm::vec2{-45.0f, +55.0f}, glm::vec2{-60.0f, +0.0f}, glm::vec2{-45.0f, -55.0f},
        //Bottom Border
        glm::vec2{-55.0f, -45.0f}, glm::vec2{+0.0f, -60.0f}, glm::vec2{+55.0f, -45.0f},    
    };

    // Normalize
    for (auto &position :positions) {
        position /= glm::vec2{50.0f, 50.0f};
    }

    m_color = {100.4f,59.3f,43.0f,0.5f};
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
  glDrawArrays(GL_TRIANGLES, 0, 12);

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