#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "declaration.hpp"

class Entity {
  private :
    sf::Sprite sprite;
    double speed;
    int life;
  public :
    sf::Vector2f get_pos(void) {return sprite.getPosition();}
    double get_rotation(void) {return sprite.getRotation();}
    void set_pos_abs(sf::Vector2f position) {sprite.setPosition(position);}
    void set_pos_abs(float x, float y) {sprite.setPosition(x,y);}
    void set_pos_rel(sf::Vector2f position) {sprite.move(position);}
    void set_pos_rel(float x, float y) {sprite.move(x,y);}
    void set_rotation_abs(double r) {sprite.setRotation(r);}
    void set_rotation_rel(double r) {sprite.rotate(r);}
    int & get_life(void) {return life;}
    sf::Sprite & get_sprite(void) {return sprite;}
    Entity(int l, int speed) :
      speed(speed),
      life(l) {}
};

#endif
