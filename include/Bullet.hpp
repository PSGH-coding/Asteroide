#ifndef BULLET_HPP
#define BULLET_HPP

#include "Entity.hpp"

class Bullet : public Entity {
  public :
    Bullet(int l, int speed) :
      Entity(l,speed) {}
};

#endif
