#pragma once
#include "../Entities/Metazoo.h"
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
  void spawnCarnivoros();

  void updateEntidades(const float &dt);

  std::vector<Entity *> plantas;
  int plantasNumber = 0;

  std::vector<Entity *> herbivoros;
  int herbivorosNumber = 1;

  std::vector<Entity *> carnivoros;
  int carnivorosNumber = 1;

  void matarEntidades();
};
