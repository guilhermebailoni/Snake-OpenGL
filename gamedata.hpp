#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Up };
enum class State { Playing, GameOver, Win };

struct GameData {
  State m_state{State::Playing};
  std::bitset<3> m_input;  // [up left, right]
};

#endif