#pragma once

#include "States/MainMenuState.h"

class Game {
public:
  Game();
  virtual ~Game();

  void endApplication();

  void updateDt();
  void updateSFMLEvents();
  void update();

  void render();

  void run();

private:
  sf::RenderWindow *window;
  sf::Event sfEvent;
  std::vector<sf::VideoMode> videoModes;

  sf::Clock dtClock;
  float dt;

  std::stack<State *> states;

  std::map<std::string, int> supportedKeys;

  void initWindows();
  void initKeys();
  void initStates();
};
