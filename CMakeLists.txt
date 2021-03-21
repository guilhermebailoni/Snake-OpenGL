project(snake)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp snake.cpp
                               walls.cpp food.cpp)
enable_abcg(${PROJECT_NAME})