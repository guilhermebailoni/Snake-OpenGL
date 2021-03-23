#include "walls.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Food::initializeGL(GLuint program, glm::vec2 m_oldTranslation) {
    terminateGL();

    m_program = program;
    m_colorLoc = glGetUniformLocation(m_program, "color");
    m_rotationLoc = glGetUniformLocation(m_program, "rotation");
    m_scaleLoc = glGetUniformLocation(m_program, "scale");
    m_translationLoc = glGetUniformLocation(m_program, "translation");
    m_pointSizeLoc = glGetUniformLocation(m_program, "pointSize");

    m_rotation = 0.0f;
    //m_translation = glm::vec2(0);
    m_velocity = glm::vec2(0);
    m_pointSize = 20.0f;

    float x = m_randomDist(m_randomEngine)*35.0f;
    float y = m_randomDist(m_randomEngine)*35.0f;

    printf("random values x: %f y: %f .", x, y);

    std::array<glm::vec2, 1> positions{
        //Food Position
        glm::vec2{x,y}
        //glm::vec2{0.0f,0.0f}
    };

    // Normalize
    for (auto &position :positions) {
        position /= glm::vec2{50.0f, 50.0f};
        position += m_oldTranslation;
    }

    m_translation = positions.front();

    positions[0].x /= 15.5f;
    positions[0].y /= 15.5f;

    printf("position x: %f y: %f .", positions[0].x, positions[0].y);
    printf("translation x: %f y: %f .", m_translation.x, m_translation.y);
    printf("old_translation x: %f y: %f .", m_oldTranslation.x, m_oldTranslation.y);

    m_color = glm::vec4{1.0f,0.55f,0.00f,1.0f};

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

void Food::paintGL() {

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  glUniform1f(m_rotationLoc, m_rotation);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);
  glUniform1f(m_pointSizeLoc, m_pointSize);

  glUniform4fv(m_colorLoc, 1, &m_color.r);


  glDrawArrays(GL_POINTS, 0, 1);

  glBindVertexArray(0);

  glUseProgram(0);
}

void Food::terminateGL() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Food::update(const Snake &snake, float deltaTime) {
    m_translation -= snake.m_velocity * deltaTime;
}