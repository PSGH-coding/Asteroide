#ifndef VAISSEAU_HPP
#define VAISSEAU_HPP

#include "declaration.hpp"
#include "Entity.hpp"
#include "Speed.hpp"

class SpaceShip : public Entity {
  private :
    int cd_tir;
    Speed speed;
  public :
    void set_cd_tir(int c) {cd_tir = c;}
    int get_cd_tir(void) {return cd_tir;}
    SpaceShip(int c, int l, int speed) :
      Entity(l,speed),
      cd_tir(c),
      speed() {}
    Speed & get_speed(void) {return speed;}
};

#endif
