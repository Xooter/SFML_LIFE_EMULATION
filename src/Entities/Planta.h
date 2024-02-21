#pragma once
#include "Entity.h"

class Planta : public Entity {
public:
  Planta(sf::Color color, float initPosX = 0.f, float initPosY = 0.f);
  ~Planta();

  void update(const float &dt) override;

private:
  float maxGrow = 5.f;
};
