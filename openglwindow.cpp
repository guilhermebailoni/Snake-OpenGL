#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }

  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
  }
  if (event.type == SDL_MOUSEMOTION) {
    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    glm::vec2 direction{glm::vec2{mousePosition.x - m_viewportWidth / 2,
                                  mousePosition.y - m_viewportHeight / 2}};
    direction.y = -direction.y;
    m_snake.setRotation(std::atan2(direction.y, direction.x) - M_PI_2);
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create program to render the circle objects
  m_circlesProgram = createProgramFromFile(getAssetsPath() + "circles.vert",
                                         getAssetsPath() + "circles.frag");
  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_score = 0;
  m_snake.initializeGL(m_circlesProgram);
  m_walls.initializeGL(m_objectsProgram);
  m_food.initializeGL(m_objectsProgram, glm::vec2(0));
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {    
    restart();
    return;
  }

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinConditions();
  }

  if(m_gameData.m_state == State::Playing){
    m_snake.update(m_gameData, deltaTime);
    m_walls.update(m_snake, deltaTime);
    m_food.update(m_snake, deltaTime);
  }
  
}

void OpenGLWindow::paintGL() {
  update();

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_walls.paintGL();
  m_food.paintGL();
  m_snake.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    auto size{ImVec2(330, 60)};
    auto position{ImVec2((m_viewportWidth * 0.05f),
                         (m_viewportHeight * 0.05f))};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoScrollbar |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    ImGui::Text("Score: %d", m_score);

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_circlesProgram);
  glDeleteProgram(m_objectsProgram);
  
  m_snake.terminateGL();
  m_walls.terminateGL();
  m_food.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // Check collision
  auto innerWall{m_walls.m_innerWall};
  innerWall /= 50.0f;

  //printf("translation test: x-%f y-%f ", m_walls.m_translation.x, m_walls.m_translation.y);

  auto distance{glm::distance(m_snake.m_translation, m_food.m_translation)};


  //checking if snake colided with food
  if(distance < m_snake.m_pointSize/m_viewportWidth + m_food.m_pointSize/m_viewportWidth){
    //getting wall translation, used for generating food inside the wall
    glm::vec2 oldTranslation = m_walls.m_translation;
    // printf("Food old translation: x:%f y%f", oldTranslation.x, oldTranslation.y);
    //printf("Wall translation: x:%f y%f", m_walls.m_translation.x, m_walls.m_translation.y);
    //deletes and generates new food
    m_food.terminateGL();
    m_food.initializeGL(m_objectsProgram, oldTranslation);
    //increases score count
    m_score++;
  }

  //checking if snake colided with wall
  if(m_walls.m_translation.x + (m_snake.m_pointSize/m_viewportWidth) > + innerWall 
   ||m_walls.m_translation.x - (m_snake.m_pointSize/m_viewportWidth) < - innerWall 
   ||m_walls.m_translation.y + (m_snake.m_pointSize/m_viewportHeight) > + innerWall 
   ||m_walls.m_translation.y - (m_snake.m_pointSize/m_viewportHeight) < - innerWall)
  {
    m_gameData.m_state = State::GameOver;
    m_restartWaitTimer.restart();
  }
}

void OpenGLWindow::checkWinConditions() {
  //max score
  if(m_score >= 999){
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}