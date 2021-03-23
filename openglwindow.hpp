#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "snake.hpp"
#include "walls.hpp"
#include "food.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_circlesProgram{};
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  Snake m_snake;
  Walls m_walls;
  Food m_food;
  int m_score = 0;

  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void checkCollisions();
  void checkWinConditions();
  void restart();
  void update();
};

#endif
