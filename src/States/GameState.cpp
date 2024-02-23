#include "GameState.h"

GameState::GameState(sf::RenderWindow *window,
                     std::map<std::string, int> *supportedKeys,
                     std::stack<State *> *states)
    : State(window, supportedKeys, states) {
  this->initKeybinds();
  this->spawnPlantas();
  this->spawnHerbivoros();
  this->spawnCarnivoros();
}

GameState::~GameState() {}

void GameState::spawnPlantas() {
  if (this->plantas.size() < this->plantasNumber) {

    for (int i = this->plantas.size(); i < this->plantasNumber; i++) {

      int initRandomX = rand() % this->window->getSize().x;
      int initRandomY = rand() % this->window->getSize().y;

      this->plantas.push_back(
          new Planta(sf::Color::Green, initRandomX, initRandomY));
    }
  } else {
    for (int i = this->plantas.size(); i > this->plantasNumber; i--) {
      this->plantas.pop_back();
    }
  }
}

void GameState::spawnHerbivoros() {
  if (this->herbivoros.size() < this->herbivorosNumber) {

    for (int i = this->herbivoros.size(); i < this->herbivorosNumber; i++) {

      int initRandomX = rand() % this->window->getSize().x;
      int initRandomY = rand() % this->window->getSize().y;

      this->herbivoros.push_back(new Metazoo(sf::Color::Blue, 1, &this->plantas,
                                             &this->carnivoros, initRandomX,
                                             initRandomY));
    }
  } else {
    for (int i = this->herbivoros.size(); i > this->herbivorosNumber; i--) {
      this->herbivoros.pop_back();
    }
  }
}

void GameState::spawnCarnivoros() {
  if (this->carnivoros.size() < this->carnivorosNumber) {

    for (int i = this->carnivoros.size(); i < this->carnivorosNumber; i++) {

      int initRandomX = rand() % this->window->getSize().x;
      int initRandomY = rand() % this->window->getSize().y;

      this->carnivoros.push_back(new Metazoo(sf::Color::Red, 5,
                                             &this->herbivoros, NULL,
                                             initRandomX, initRandomY));
    }
  } else {
    for (int i = this->carnivoros.size(); i > this->carnivorosNumber; i--) {
      this->carnivoros.pop_back();
    }
  }
}

void GameState::matarEntidades() {

  auto it = herbivoros.begin();
  while (it != herbivoros.end()) {
    if (!(*it)->isAlive()) {
      delete *it;
      it = herbivoros.erase(it);
    } else {
      ++it;
    }
  }

  auto itC = carnivoros.begin();
  while (itC != carnivoros.end()) {
    if (!(*itC)->isAlive()) {
      delete *itC;
      itC = carnivoros.erase(itC);
    } else {
      ++itC;
    }
  }
}

void GameState::initKeybinds() {

  std::ifstream ifs("config/gamestate_keys.ini");

  if (ifs.is_open()) {
    std::string key = "";
    std::string key_value = "";
    while (ifs >> key >> key_value) {
      this->keybinds[key] = this->supportedKeys->at(key_value);
    }
  }
  ifs.close();
}

void GameState::updateInput(const float &dt) {

  if (sf::Keyboard::isKeyPressed(
          sf::Keyboard::Key(this->keybinds.at("CLOSE")))) {
    this->endState();
  }
}

void GameState::update(const float &dt) {
  this->updateMousePositions();
  this->updateInput(dt);

  this->matarEntidades();

  this->updateEntidades(dt);

  ImguiMenu();
}

void GameState::updateEntidades(const float &dt) {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(1920, 1080));
  ImGui::Begin("a", nullptr,
               ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoResize);

  for (auto planta : this->plantas) {
    planta->update(dt);
  }

  for (auto her : this->herbivoros) {
    her->update(dt);
  }

  for (auto car : this->carnivoros) {
    car->update(dt);
  }

  ImGui::End();
}

void GameState::ImguiMenu() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  sf::Vector2u size = this->window->getSize();
  ImGui::SetNextWindowSize(ImVec2(400, size.y));
  ImGui::Begin("Specs", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoNav);

  ImGui::SliderInt("Plantas", &this->plantasNumber, 0, 100);
  ImGui::SliderInt("Herbivoro", &this->herbivorosNumber, 0, 500);
  ImGui::SliderInt("Carnivoros", &this->carnivorosNumber, 0, 200);

  if (ImGui::Button("Spawn")) {
    this->spawnPlantas();
    this->spawnHerbivoros();
    this->spawnCarnivoros();
  }

  ImGui::Dummy(ImVec2(0.0f, 20.0f));
  ImGui::Text("Stats");
  if (!this->plantas.empty())
    ImGui::Text("Plantas: %d", (int)this->plantas.size());
  if (!this->herbivoros.empty())
    ImGui::Text("Herbivoros: %d", (int)this->herbivoros.size());
  if (!this->carnivoros.empty())
    ImGui::Text("Carnivoros: %d", (int)this->carnivoros.size());

  ImGui::End();
}

void GameState::render(sf::RenderTarget *target) {

  if (!target)
    target = this->window;

  sf::RectangleShape background = sf::RectangleShape(
      sf::Vector2f(this->window->getSize().x, this->window->getSize().y));

  background.setFillColor(sf::Color(27, 27, 27));
  target->draw(background);

  for (auto planta : this->plantas) {
    planta->render(target);
  }

  for (auto her : this->herbivoros) {
    her->render(target);
  }

  for (auto car : this->carnivoros) {
    car->render(target);
  }
}
