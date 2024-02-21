#include "Planta.h"

Planta::Planta(sf::Color color, float initPosX, float initPosY)
    : Entity(color, initPosX, initPosY) {
  this->shape.setSize(sf::Vector2f(5.f, 5.f));
  this->maxGrow = rand() % 3 + 5;
}

Planta::~Planta() {}

void Planta::update(const float &dt) {
  Entity::update(dt);

  float scaleFactor = std::min(this->maxGrow, 1.f + std::max(0.f, this->age));
  this->shape.setSize(sf::Vector2f(5.f * scaleFactor, 5.f * scaleFactor));
}
