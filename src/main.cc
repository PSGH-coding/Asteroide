#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "../include/Entity.hpp"
#include "../include/SpaceShip.hpp"
#include "../include/Asteroid.hpp"
#include "../include/Bullet.hpp"
#include "../include/Texture_Group.hpp"
#include "../include/Keys.hpp"
#include "../include/Game.hpp"

int main(void){
  Game game(SIZE_OF_WINDOW_X,SIZE_OF_WINDOW_Y,"Astéroïde");
  game.run();
  game.clean();
  //sf::sleep(sf::milliseconds(1000));
  return 0;
}
