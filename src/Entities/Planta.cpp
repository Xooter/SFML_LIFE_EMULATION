#include "Planta.h"

Planta::Planta(sf::Color color, float initPosX, float initPosY)
    : Entity(color, initPosX, initPosY) {
  this->shape.setSize(sf::Vector2f(5.f, 5.f));
  this->maxGrow = rand() % 3 + 5;
}

Planta::~Planta() {}

void Planta::update(const float &dt) {
  Entity::update(dt);

  float scaleFactor =
      std::min(this->maxGrow, 1.f + std::max(0.f, this->health - 100.f));
  this->shape.setSize(sf::Vector2f(5.f * scaleFactor, 5.f * scaleFactor));
}

void Planta::envejecer(const float &dt) {
  Entity::envejecer(dt);

  this->health = std::min(200.f, this->health + 0.1f);
}
