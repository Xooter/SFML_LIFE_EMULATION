#pragma once
#include "Entity.h"
#include "Planta.h"

class Metazoo : public Entity {
public:
  Metazoo(sf::Color color, float comidaPotenciador,
          std::vector<Entity *> *comida, std::vector<Entity *> *predadores,
          float initPosX = 0.f, float initPosY = 0.f);
  ~Metazoo();

  void move(float x, float y, const float &dt, float speedBoost = 1.f);
  void update(const float &dt) override;

  void setState(Estado estado);

protected:
  int idlingTimeRand = 3;

  sf::Vector2f randomLocation;

  std::vector<Entity *> *comida;
  Entity *comidaActual = nullptr;
  float comidaPotenciador;

  std::vector<Entity *> *predadores;

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

private:
  Entity *getClosestEntity(std::vector<Entity *> *entities,
                           float ageRestriction = 0);
};
