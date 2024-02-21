#include "Entity.h"

Entity::Entity(sf::Color color, float initPosX, float initPosY) {
  this->shape.setSize(sf::Vector2f(5.f, 5.f));
  this->shape.setPosition(initPosX, initPosY);
  this->color = color;
  int randomSex = std::max(0, rand() % 3 - 1);
  this->sex = randomSex;
  this->shape.setFillColor(this->color);
}

Entity::~Entity() {}

double Entity::distanceTo(Entity *entity) {
  return sqrt(
      std::pow(this->shape.getPosition().x - entity->getPosition().x, 2) +
      std::pow(this->shape.getPosition().y - entity->getPosition().y, 2));
}
double Entity::distanceTo(sf::Vector2f pos) {
  return sqrt(std::pow(this->shape.getPosition().x - pos.x, 2) +
              std::pow(this->shape.getPosition().y - pos.y, 2));
}

sf::Vector2f Entity::getPosition() { return this->shape.getPosition(); }

void Entity::update(const float &dt) {
  if (this->health <= 0) {
    this->morir();
  }

  envejecer(dt);
}

void Entity::render(sf::RenderTarget *target) { target->draw(this->shape); }

void Entity::morir() { this->alive = false; }

void Entity::envejecer(const float &dt) {
  float deltaTime = this->clock.getElapsedTime().asSeconds();
  if (deltaTime >= 1.0f) {
    this->clock.restart();

    float hunger_per_tick = 2.f;
    if (this->estado != DURMIENDO) {
      this->energy -= hunger_per_tick * this->energyReduction;
    }
    this->hunger += hunger_per_tick;
    this->age += 0.1f;
  }
}

bool Entity::isAlive() { return this->alive; }
