#include "MainMenuState.h"

MainMenuState::MainMenuState(sf::RenderWindow *window,
                             std::map<std::string, int> *supportedKeys,
                             std::stack<State *> *states)
    : State(window, supportedKeys, states) {
  this->initFonts();
  this->initKeybinds();
  this->initButtons();

  this->background.setSize(
      sf::Vector2f(window->getSize().x, window->getSize().y));
  this->background.setFillColor(sf::Color::Black);
}

MainMenuState::~MainMenuState() {
  auto it = this->buttons.begin();
  for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
    delete it->second;
  }
}

void MainMenuState::initFonts() {
  if (!this->font.loadFromFile("fonts/noto.ttf")) {
    throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
  }
}

void MainMenuState::initButtons() {

  const float x_mitad = this->window->getSize().x / 2.f - (200 / 2.f);
  const float y_mitad = this->window->getSize().y / 2.f - (50 / 2.f);

  this->buttons["GAME_STATE"] =
      new Button(x_mitad, 100, 200, 50, &this->font, "New", sf::Color::Magenta,
                 sf::Color::Blue, sf::Color::Blue);
  this->buttons["EXIT_STATE"] =
      new Button(x_mitad, 200, 200, 50, &this->font, "Quit", sf::Color::Magenta,
                 sf::Color::Blue, sf::Color::Blue);
}

void MainMenuState::initKeybinds() {

  std::ifstream ifs("config/mainmenu_keys.ini");

  if (ifs.is_open()) {
    std::string key = "";
    std::string key_value = "";
    while (ifs >> key >> key_value) {
      this->keybinds[key] = this->supportedKeys->at(key_value);
    }
  }
  ifs.close();
}

void MainMenuState::updateInput(const float &dt) {}

void MainMenuState::update(const float &dt) {
  this->updateInput(dt);
  this->updateMousePositions();

  this->updateButtons();

  if (this->buttons["GAME_STATE"]->isPressed()) {
    this->states->push(
        new GameState(this->window, this->supportedKeys, this->states));
  }

  if (this->buttons["EXIT_STATE"]->isPressed()) {
    this->endState();
  }
}

void MainMenuState::updateButtons() {
  for (auto &it : this->buttons) {
    it.second->update(this->mousePosView);
  }
}

void MainMenuState::render(sf::RenderTarget *target) {

  if (!target)
    target = this->window;

  target->draw(this->background);

  this->renderButtons(target);
}

void MainMenuState::renderButtons(sf::RenderTarget *target) {
  for (auto &it : this->buttons) {
    it.second->render(target);
  }
}
