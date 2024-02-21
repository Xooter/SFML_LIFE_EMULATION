#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

#include "../imgui/imgui-SFML.h"
#include "../imgui/imgui.h"

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

class Entity {
private:
  bool alive = true;
  void envejecer(const float &dt);

protected:
  sf::Clock clock;
  enum Estado {
    IDLE = 0,
    BUSCANDO_COMIDA,
    COMIENDO,
    HUYENDO,
    DURMIENDO,
    BUSCANDO_PAREJA,
    REPRODUCIENDO
  };

  sf::RectangleShape shape;

  float health = 100;        // vida
  float energy = 100;        // energia
  float hunger = 0;          // hambre
  float energyReduction = 1; // sed
  sf::Color color;

  Estado estado;

public:
  Entity(sf::Color color, float initPosX = 0.f, float initPosY = 0.f);
  ~Entity();

  int sex;          // sexo 0 = hembra, 1 = macho
  float speed = 1;  // velocidad
  float vision = 0; // vision

  float age = 0; // edad

  float reproductionAgeMax;
  float reproductionAgeMin;

  virtual double distanceTo(Entity *entity);
  virtual double distanceTo(sf::Vector2f position);
  virtual sf::Vector2f getPosition();

  virtual void update(const float &dt);
  virtual void render(sf::RenderTarget *target);

  void morir();
  bool isAlive();
};
