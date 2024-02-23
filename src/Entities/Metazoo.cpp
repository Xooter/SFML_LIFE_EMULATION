#include "Metazoo.h"

Metazoo::Metazoo(sf::Color color, float comidaPotenciador,
                 std::vector<Entity *> *comida,
                 std::vector<Entity *> *predadores, float initPosX,
                 float initPosY)
    : Entity(color, initPosX, initPosY) {

  this->comida = comida;
  this->comidaPotenciador = comidaPotenciador;
  this->predadores = predadores;

  this->estado = IDLE;
  this->waitToMove = rand() % 2 + 1;
  this->walkingTime = 0;

  this->reproductionAgeMin = 6;
  this->reproductionAgeMax = 25;

  this->timeToSleep = rand() % 16 + 15; // entre 30 y 15
}

Metazoo::~Metazoo() {}

void Metazoo::move(float x, float y, const float &dt, float speedBoost) {
  float speed = this->speed;
  if (this->age < 6)
    speed = this->speed * 0.6;
  if (this->age > 30)
    speed = this->speed * 0.4;
  if (this->age > 60)
    speed = this->speed * 0.2;

  double magnitud = sqrt(x * x + y * y);
  if (magnitud > 0) {
    x /= magnitud;
    y /= magnitud;
  }

  this->shape.move(20 * x * (speed * speedBoost) * dt,
                   20 * y * (speed * speedBoost) * dt);
}

void Metazoo::setState(Estado estado) { this->estado = estado; }

void Metazoo::update(const float &dt) {
  Entity::update(dt);

  this->debug();

  if (this->energy <= 0 && this->hunger >= 100 || this->health <= 0)
    this->morir();

  if (this->energy < this->timeToSleep) {
    this->setState(DURMIENDO);
  } else if (this->hunger > 10.f * this->comidaPotenciador &&
             this->estado != COMIENDO &&
             (this->estado != DURMIENDO || this->energy > 80) &&
             !this->comida->empty()) {
    this->setState(BUSCANDO_COMIDA);
  }

  if (this->predadores != NULL && this->estado != COMIENDO) {
    for (auto predador : *this->predadores) {
      if (this->distanceTo(predador->getPosition()) < this->vision &&
          (this->estado != DURMIENDO ||
           this->distanceTo(predador->getPosition()) < this->vision / 2) &&
          predador != this) {
        this->setState(HUYENDO);
      }
    }
  }

  switch (this->estado) {
  case IDLE:
    idle(dt);
    break;
  case BUSCANDO_COMIDA:
    buscarComida(dt);
    break;
  case COMIENDO:
    comer(dt);
    break;
  case HUYENDO:
    huir(dt);
    break;
  case DURMIENDO:
    dormir(dt);
    break;
  case BUSCANDO_PAREJA:
    buscarPareja(dt);
    break;
  case REPRODUCIENDO:
    reproducirse(dt);
    break;
  }
}

void Metazoo::idle(const float &dt) {
  this->waitToMove -= dt;
  this->walkingTime -= dt;

  if (this->waitToMove <= 0) {
    float randomX =
        static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 2.0f -
        1.0f;
    float randomY =
        static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 2.0f -
        1.0f;

    this->randomLocation = sf::Vector2f(randomX, randomY);
  }

  if (this->waitToMove <= 0 && this->walkingTime <= 0) {
    this->waitToMove = std::max(10, rand() % 20 + 1);
    this->walkingTime = std::max(5, rand() % 10 + 1);
  }

  if (this->walkingTime > 0 && this->getPosition().x < 1920 &&
      this->getPosition().x > 0 && this->getPosition().y < 1080 &&
      this->getPosition().y > 0) {

    this->move(this->randomLocation.x, this->randomLocation.y, dt);
  }
}

Entity *Metazoo::getClosestEntity(std::vector<Entity *> *entities,
                                  float ageRestriction) {
  Entity *closestEntity = entities->at(0);

  for (auto entity : *entities) {
    if (entity != this &&
        this->distanceTo(entity->getPosition()) <
            this->distanceTo(closestEntity) &&
        entity->age > ageRestriction) {
      closestEntity = entity;
    }
  }

  return closestEntity;
}

void Metazoo::buscarComida(const float &dt) {

  this->comidaActual = this->getClosestEntity(this->comida, 1.f);

  double diferenciaX =
      (this->comidaActual->getPosition().x - this->getPosition().x);
  double diferenciaY =
      (this->comidaActual->getPosition().y - this->getPosition().y);

  this->move(diferenciaX, diferenciaY, dt, 1.8f);

  float randomDistance = rand() % 5;
  if (this->distanceTo(this->comidaActual->getPosition()) < randomDistance)
    this->setState(COMIENDO);
}

void Metazoo::comer(const float &dt) {
  if (this->comidaActual == nullptr || this->comidaActual->age <= 1.f) {
    this->setState(BUSCANDO_COMIDA);
  }

  sf::Color color = this->color;
  int color_increment = 100;
  color.r = std::min(255, color.r + color_increment);
  color.g = std::min(255, color.g + color_increment);
  color.b = std::min(255, color.b + color_increment);
  this->shape.setFillColor(color);

  float deltaTime = this->clock.getElapsedTime().asSeconds();
  if (deltaTime >= .9f) {
    this->hunger = std::max(0.f, this->hunger - 10.f * comidaPotenciador);
    this->comidaActual->health -=
        0.8f * comidaPotenciador * this->comidaActual->health / 100;
  }

  if (this->hunger <= 20) {
    this->shape.setFillColor(this->color);
    setState(IDLE);
  }
}

void Metazoo::huir(const float &dt) {
  Entity *predador = this->getClosestEntity(this->predadores, 0.f);
  if (predador == nullptr || this->distanceTo(predador) > this->vision ||
      predador == this) {
    setState(IDLE);
    return;
  }

  double diferenciaX = (predador->getPosition().x - this->getPosition().x);
  double diferenciaY = (predador->getPosition().y - this->getPosition().y);

  diferenciaX = -diferenciaX;
  diferenciaY = -diferenciaY;

  this->move(diferenciaX, diferenciaY, dt, this->energy / 100.0);

  if (this->distanceTo(predador) > this->vision)
    this->setState(IDLE);
}

void Metazoo::dormir(const float &dt) {
  this->shape.setFillColor(
      sf::Color(this->color.r, this->color.g, this->color.b, 150));

  // mientras mas hambre, mas le cuenta recuperarse
  // los mas alimentados mejor descansan
  float factorAtenuacion = 1.0f - (this->hunger / 80.0f);
  this->energy = std::min(100.f, this->energy + 5.f * factorAtenuacion * dt);

  if (this->energy >= 100) {
    this->shape.setFillColor(
        sf::Color(this->color.r, this->color.g, this->color.b, 255));

    this->energyReduction = 0.8f;
    this->setState(IDLE);
  }
}

void Metazoo::buscarPareja(const float &dt) {}
void Metazoo::reproducirse(const float &dt) {}

void Metazoo::debug() {
  sf::FloatRect bounds = this->shape.getGlobalBounds();
  int bound_added = 60;
  bounds.left += bound_added / 2.f;
  bounds.top += bound_added / 2.f;
  bounds.width -= bound_added;
  bounds.height -= bound_added;
  if (bounds.contains(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)) {

    ImGui::SetCursorPosX(this->getPosition().x);
    ImGui::SetCursorPosY(this->getPosition().y - 16);
    std::string estadoActual = "";
    switch (this->estado) {
    case IDLE:
      estadoActual = "IDLE";
      break;
    case BUSCANDO_COMIDA:
      estadoActual = "BUSCANDO_COMIDA";
      break;
    case COMIENDO:
      estadoActual = "COMIENDO";
      break;
    case HUYENDO:
      estadoActual = "HUYENDO";
      break;
    case DURMIENDO:
      estadoActual = "DURMIENDO";
      break;
    case BUSCANDO_PAREJA:
      estadoActual = "BUSCAR_PAREJA";
      break;
    case REPRODUCIENDO:
      estadoActual = "REPRODUCIENDO";
      break;
    }

    ImGui::Text("%s", estadoActual.c_str());
    ImGui::SetCursorPosX(this->getPosition().x);
    ImGui::SetCursorPosY(this->getPosition().y - 32);
    ImGui::Text("Hambre: %.1f, Edad: %.1f", this->hunger, this->age);

    ImGui::SetCursorPosX(this->getPosition().x);
    ImGui::SetCursorPosY(this->getPosition().y - 48);
    ImGui::Text("Sexo: %s, Energia: %.1f", this->sex == 0 ? "H" : "M",
                this->energy);

    if (this->estado == IDLE) {
      ImGui::SetCursorPosX(this->getPosition().x);
      ImGui::SetCursorPosY(this->getPosition().y + 16);
      ImGui::Text("wtm: %.2f ,wt: %.2f", this->waitToMove, this->walkingTime);
    }
  }
}
