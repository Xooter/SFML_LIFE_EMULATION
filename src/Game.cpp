#include "Game.h"
#include "imgui/imgui-SFML.h"

void Game::initKeys() {

  std::ifstream ifs("config/keys.ini");

  if (ifs.is_open()) {
    std::string key = "";
    int key_value = 0;
    while (ifs >> key >> key_value) {
      this->supportedKeys[key] = key_value;
    }
  }
  ifs.close();
}

void Game::initStates() {
  this->states.push(
      new MainMenuState(this->window, &this->supportedKeys, &this->states));
}

void Game::initWindows() {

  this->videoModes = sf::VideoMode::getFullscreenModes();
  sf::VideoMode window_bounds = sf::VideoMode::getDesktopMode();
  bool fullscreen = true;
  unsigned framerate_limit = 120;
  bool vertical_sync_enabled = false;
  unsigned antialiasing_level = 0;

  std::string title = "SFML works!";

  sf::ContextSettings window_settings;
  window_settings.antialiasingLevel = antialiasing_level;

  if (fullscreen)
    this->window = new sf::RenderWindow(window_bounds, title,
                                        sf::Style::Fullscreen, window_settings);
  else
    this->window = new sf::RenderWindow(window_bounds, title,
                                        sf::Style::Titlebar | sf::Style::Close,
                                        window_settings);

  this->window->setFramerateLimit(framerate_limit);
  this->window->setVerticalSyncEnabled(vertical_sync_enabled);

  auto _ = ImGui::SFML::Init(*this->window, true);
}

Game::Game() {
  this->initWindows();
  this->initKeys();
  this->initStates();
}

Game::~Game() {
  delete this->window;
  while (!this->states.empty()) {
    delete this->states.top();
    this->states.pop();
  }

  ImGui::SFML::Shutdown();
}

void Game::endApplication() {
  std::cout << "Ending Application"
            << "\n";
}

void Game::updateDt() {
  this->dt =
      this->dtClock.restart().asSeconds() * this->states.top()->clockVelocity;
}

void Game::updateSFMLEvents() {
  while (this->window->pollEvent(this->sfEvent)) {
    ImGui::SFML::ProcessEvent(*this->window, this->sfEvent);
    if (this->sfEvent.type == sf::Event::Closed) {
      this->window->close();
    }
  }
}

void Game::update() {
  this->updateSFMLEvents();

  ImGui::SFML::Update(*this->window, dtClock.restart());

  if (!this->states.empty()) {

    this->states.top()->update(this->dt);

    if (this->states.top()->getQuit()) {
      this->states.top()->endState();
      delete this->states.top();
      this->states.pop();
    }
  } else {
    endApplication();
    this->window->close();
  }
}

void Game::render() {

  this->window->clear();

  if (!this->states.empty()) {
    this->states.top()->render(this->window);
  }

  ImGui::SFML::Render(*this->window);
  this->window->display();
}

void Game::run() {
  while (this->window->isOpen()) {
    this->updateDt();
    this->update();
    this->render();
  }
}
