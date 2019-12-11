#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "declaration.hpp"

class Explosion{
  private :
    unsigned int frame;
    sf::Vector2f position;
  public :
    Explosion(unsigned int i = 0) :
      frame(i),
      position(0,0) {}
    void set_frame(unsigned int i) {frame = i;}
    void set_frame_plus_1(void) {frame++;}
    unsigned int get_frame(void) {return frame;}
    void set_position(sf::Vector2f p) {position = p;}
    sf::Vector2f & get_position(void) {return position;}
};

#endif
