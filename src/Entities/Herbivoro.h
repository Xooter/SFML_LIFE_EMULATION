#pragma once
#include "Entity.h"
#include "Planta.h"

class Herbivoro : public Entity {
public:
  Herbivoro(sf::Color color, std::vector<Planta *> *comida,
            float initPosX = 0.f, float initPosY = 0.f);
  ~Herbivoro();

  void move(float x, float y);
  void update(const float &dt) override;

  void setState(Estado estado);

private:
  int idlingTimeRand = 3;

  sf::Vector2f randomLocation;

  std::vector<Planta *> *comida;
  Planta *comidaActual = nullptr;

  float waitToMove;
  float walkingTime;

  float timeToSleep = 20;

  void debug();

  void idle(const float &dt);
  void buscarComida(const float &dt);
  void comer(const float &dt);
  void huir(const float &dt);
  void dormir(const float &dt);
  void buscarPareja(const float &dt);
  void reproducirse(const float &dt);
};
