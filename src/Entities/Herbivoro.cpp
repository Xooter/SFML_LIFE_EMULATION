#include "Herbivoro.h"

Herbivoro::Herbivoro(sf::Color color, std::vector<Planta *> *comida,
                     float initPosX, float initPosY)
    : Entity(color, initPosX, initPosY) {

  this->comida = comida;

  this->estado = IDLE;
  this->waitToMove = rand() % 2 + 1;
  this->walkingTime = 0;

  this->reproductionAgeMin = 6;
  this->reproductionAgeMax = 25;

  this->timeToSleep = rand() % 16 + 15; // entre 30 y 15
}

Herbivoro::~Herbivoro() {}

void Herbivoro::move(float x, float y) {
  float speed = this->speed;
  if (this->age < 6)
    speed = this->speed * 0.6;
  if (this->age > 30)
    speed = this->speed * 0.4;
  if (this->age > 60)
    speed = this->speed * 0.2;

  this->shape.move(x * speed, y * speed);
}

void Herbivoro::setState(Estado estado) { this->estado = estado; }

void Herbivoro::update(const float &dt) {
  Entity::update(dt);

  this->debug();

  if (this->energy <= 0 && this->hunger >= 100)
    this->morir();

  if (this->energy < this->timeToSleep) {
    this->setState(DURMIENDO);
  } else if (this->hunger > 20 && this->estado != COMIENDO) {
    this->setState(BUSCANDO_COMIDA);
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

void Herbivoro::idle(const float &dt) {
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

    this->move(20 * this->randomLocation.x * dt,
               20 * this->randomLocation.y * dt);
  }
}

void Herbivoro::buscarComida(const float &dt) {
  if (this->comida->empty())
    setState(IDLE);

  for (auto planta : *this->comida) {
    if (this->comidaActual == nullptr ||
        (this->distanceTo(planta->getPosition()) <
         this->distanceTo(this->comidaActual)) &&
            planta->age > 1.f) {
      this->comidaActual = planta;
    }
  }

  double diferenciaX =
      (this->comidaActual->getPosition().x - this->getPosition().x);
  double diferenciaY =
      (this->comidaActual->getPosition().y - this->getPosition().y);

  // Normalizar la diferencia
  double magnitud = sqrt(diferenciaX * diferenciaX + diferenciaY * diferenciaY);
  if (magnitud > 0) {
    diferenciaX /= magnitud;
    diferenciaY /= magnitud;
  }

  this->move(20 * diferenciaX * dt, 20 * diferenciaY * dt);

  float randomDistance = rand() % 15 + 5;
  if (this->distanceTo(this->comidaActual->getPosition()) < randomDistance)
    this->setState(COMIENDO);
}

void Herbivoro::comer(const float &dt) {
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
    this->hunger = std::max(0.f, this->hunger - 10.f);
    this->comidaActual->age -= 0.8f;
  }

  if (this->hunger <= 20) {
    this->shape.setFillColor(this->color);
    setState(IDLE);
  }
}
void Herbivoro::huir(const float &dt) {}
void Herbivoro::dormir(const float &dt) {
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

void Herbivoro::buscarPareja(const float &dt) {}
void Herbivoro::reproducirse(const float &dt) {}

void Herbivoro::debug() {
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
