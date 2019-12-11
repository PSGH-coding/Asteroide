#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "declaration.hpp"
#include "Entity.hpp"

class Asteroid : public Entity {
  private :
    double rotation;
  public :
    Asteroid(int l, int speed) :
      Entity(l,speed),
      rotation(0) {}
    void set_rot(double r) {rotation = r;}
    double & get_rot(void) {return rotation;}
};

#endif
