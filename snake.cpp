#include "snake.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Snake::initializeGL(GLuint program) {
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
    m_pointSize = 40.0f;

    std::array<glm::vec2, 5> positions{
        //Snake Body
        glm::vec2{+0.0f, +0.0f},
        //Snake Right Eye
        glm::vec2{+1.5f, +1.0f}, glm::vec2{+1.5f, +0.0f},
        //Snake Left Eye
        glm::vec2{-1.5f, +1.0f}, glm::vec2{-1.5f, +0.0f},
    };

    // Normalize
    for (auto &position :positions) {
        position /= glm::vec2{15.5f, 15.5f};
    }

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

void Snake::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  glUniform1f(m_rotationLoc, m_rotation);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);
  glUniform1f(m_pointSizeLoc, m_pointSize);

  glUniform4fv(m_colorLoc, 1, &m_color.r);


glDrawArrays(GL_POINTS, 0, 1);
glDrawArrays(GL_LINES, 1, 4);

  glBindVertexArray(0);

  glUseProgram(0);
}

void Snake::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
}

void Snake::update(const GameData &gameData, float deltaTime) {
    // Rotate
    if (gameData.m_input[static_cast<size_t>(Input::Left)])
        m_rotation = glm::wrapAngle(m_rotation + 5.0f * deltaTime);
    if (gameData.m_input[static_cast<size_t>(Input::Right)])
        m_rotation = glm::wrapAngle(m_rotation - 5.0f * deltaTime);

    // Apply thrust
    if (gameData.m_input[static_cast<size_t>(Input::Up)] &&
        gameData.m_state == State::Playing) {
        // More speed in the forward vector
        glm::vec2 forward = glm::rotate(glm::vec2{0.0f, 1.0f}, m_rotation);
        m_velocity = (forward * 250.0f) * deltaTime;
    } else {
        // Just moves
        glm::vec2 forward = glm::rotate(glm::vec2{0.0f, 1.0f}, m_rotation);
        m_velocity = forward * 100.0f * deltaTime;
    }
}