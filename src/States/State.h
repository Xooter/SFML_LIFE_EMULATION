#pragma once

#include "../Entities/Entity.h"
#include "../Entities/Herbivoro.h"
#include "../Entities/Planta.h"

class State {
public:
  State(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys,
        std::stack<State *> *states);
  virtual ~State();

  float clockVelocity = 1;

  const bool &getQuit() const;

  void endState();
  virtual void updateMousePositions();
  virtual void updateInput(const float &dt) = 0;
  virtual void update(const float &dt) = 0;
  virtual void render(sf::RenderTarget *target = NULL) = 0;

protected:
  std::stack<State *> *states;
  sf::RenderWindow *window;
  std::map<std::string, int> *supportedKeys;
  std::map<std::string, int> keybinds;
  bool quit;

  sf::Vector2i mousePosScreen;
  sf::Vector2i mousePosWindow;
  sf::Vector2f mousePosView;

  std::vector<sf::Texture> textures;

  virtual void initKeybinds() = 0;
};
