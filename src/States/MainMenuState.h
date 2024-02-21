#pragma once
#include "../UI/Button.h"
#include "GameState.h"

class MainMenuState : public State {
public:
  MainMenuState(sf::RenderWindow *window,
                std::map<std::string, int> *supportedKeys,
                std::stack<State *> *states);
  virtual ~MainMenuState();

  void updateInput(const float &dt);
  void update(const float &dt);
  void updateButtons();
  void render(sf::RenderTarget *target = NULL);
  void renderButtons(sf::RenderTarget *target);

private:
  sf::RectangleShape background;
  sf::Font font;

  std::map<std::string, Button *> buttons;

  void initFonts();
  void initKeybinds();
  void initButtons();
};
