#pragma once
#include "State.h"

class GameState : public State {
public:
  GameState(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys,
            std::stack<State *> *states);
  virtual ~GameState();

  void updateInput(const float &dt);
  void update(const float &dt);
  void render(sf::RenderTarget *target = nullptr);

private:
  void initKeybinds();
  void ImguiMenu();

  void spawnPlantas();
  void spawnHerbivoros();
  void updateEntidades(const float &dt);

  std::vector<Planta *> plantas;
  int plantasNumber = 0;

  std::vector<Herbivoro *> herbivoros;
  int herbivorosNumber = 1;

  void matarEntidades();
};
