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

int rand_a_b(int a, int b){
  return rand() % ( b - a + 1 ) + a;
}

void Game::check_key_pressed(void){
  if (event.key.code == sf::Keyboard::Z){
    keys.setZ_key(true);
  }
  else if (event.key.code == sf::Keyboard::S){
    keys.setS_key(true);
  }
  else if (event.key.code == sf::Keyboard::D){
    keys.setD_key(true);
  }
  else if (event.key.code == sf::Keyboard::Q){
    keys.setQ_key(true);
  }
  else if (event.key.code == sf::Keyboard::Space){
    keys.setSPACE_key(true);
  }
}

void Game::check_key_released(void){
  if (event.key.code == sf::Keyboard::Z){
    keys.setZ_key(false);
  }
  else if (event.key.code == sf::Keyboard::S){
    keys.setS_key(false);
  }
  else if (event.key.code == sf::Keyboard::D){
    keys.setD_key(false);
  }
  else if (event.key.code == sf::Keyboard::Q){
    keys.setQ_key(false);
  }
  else if (event.key.code == sf::Keyboard::Space){
    keys.setSPACE_key(false);
  }
}

void Game::key_move(void){
  int translation_z = 0;
  int translation_s = 0;
  int rotation_d = 0;
  int rotation_q = 0;
  double rotation = space_ship.get_sprite().getRotation();

  space_ship.get_speed().speed_x *= SPEED_D_ACCEL_X_Y_SPACE_SHIP;
  space_ship.get_speed().speed_y *= SPEED_D_ACCEL_X_Y_SPACE_SHIP;
  space_ship.get_speed().speed_r *= SPEED_D_ACCEL_R_SPACE_SHIP;
  rotation = ( PI * rotation ) / 180;

  if (keys.getZ_key() == true)  translation_z = 1;
  else translation_z = 0;

  if (keys.getS_key() == true)  translation_s = 1;
  else translation_s = 0;

  if (keys.getD_key() == true)  rotation_d = 1;
  else rotation_d = 0;

  if (keys.getQ_key() == true)  rotation_q = 1;
  else rotation_q = 0;

  space_ship.get_speed().speed_x += sin(rotation) * (translation_z - translation_s) * SPEED_SPACE_SHIP;
  space_ship.get_speed().speed_y += ( - translation_z + translation_s ) * cos(rotation) * SPEED_SPACE_SHIP;
  space_ship.get_speed().speed_r += ( rotation_d - rotation_q ) * SPEED_ROTATION;

  space_ship.get_sprite().move(space_ship.get_speed().speed_x, space_ship.get_speed().speed_y);
  space_ship.get_sprite().rotate(space_ship.get_speed().speed_r);

  if (keys.getSPACE_key() == true){
    if (space_ship.get_cd_tir() == 0){
      new_bullet();
      space_ship.set_cd_tir(CD_TIR);
    }
    else {
      space_ship.set_cd_tir(space_ship.get_cd_tir()-1);
    }
  }
  if_space_ship_is_out_screen();
}

void Game::ProcessEvents(void){
  while (window.pollEvent(event)){
    if (event.type == sf::Event::Closed) window.close();
    else if (event.type == sf::Event::KeyPressed) check_key_pressed();
    else if (event.type == sf::Event::KeyReleased) check_key_released();
  }
}

void Game::if_space_ship_is_out_screen(void){
  sf::Vector2f position = space_ship.get_pos();
  if (position.y < 0) space_ship.set_pos_abs(position.x,SIZE_OF_WINDOW_Y);
  else if (position.y > SIZE_OF_WINDOW_Y) space_ship.set_pos_abs(position.x,0);
  if (position.x < 0) space_ship.set_pos_abs(SIZE_OF_WINDOW_X,position.y);
  else if (position.x > SIZE_OF_WINDOW_X) space_ship.set_pos_abs(0,position.y);
}

void Game::if_bullet_is_out_of_screen(Bullet * bullet, int i){
  sf::Vector2f position = bullet->get_pos();
  if (position.y < 0 || position.y > SIZE_OF_WINDOW_Y || position.x < 0 || position.x > SIZE_OF_WINDOW_X) {
    delete bullet;
    bullets.erase(begin(bullets)+i);
  }
}

void Game::if_asteroid_is_out_of_screen(Asteroid * asteroid){
  sf::Vector2f position = asteroid->get_pos();
  if (position.y < 0) asteroid->set_pos_abs(position.x,SIZE_OF_WINDOW_Y);
  else if (position.y > SIZE_OF_WINDOW_Y) asteroid->set_pos_abs(position.x,0);
  if (position.x < 0) asteroid->set_pos_abs(SIZE_OF_WINDOW_X,position.y);
  else if (position.x > SIZE_OF_WINDOW_X) asteroid->set_pos_abs(0,position.y);
}

void Game::run(void){
  while (window.isOpen()){
    ProcessEvents();
    update();
    render();
  }
}

void Game::update(void){
  if ( game_over == false ) {
    check_nb_asteroid();
    key_move();
    bullets_move();
    asteroids_move();
    test_collisions();
    test_game_over();
  }
  else final_explosion();
}

void Game::clean_explosion(unsigned int i){
      delete explosions[i];
      explosions.erase(explosions.begin() + i);
}

void Game::test_collisions(void){

  float size_asteroid = MIN(size_texture_asteroid.x/2,size_texture_asteroid.y/2);
  float pos_x_space_ship;
  float pos_y_space_ship;
  float pos_x_asteroid;
  float pos_y_asteroid;
  float pos_x_bullet;
  float pos_y_bullet;

  for (unsigned int i = 0; i < bullets.size(); i++){
    for (unsigned int j = 0; j < asteroids.size(); j++){
      pos_x_bullet = bullets[i]->get_pos().x + sin(bullets[i]->get_rotation() * PI/180) * size_texture_bullet.y/4;
      pos_y_bullet = bullets[i]->get_pos().y - cos(bullets[i]->get_rotation() * PI/180) * size_texture_bullet.y/4;
      pos_x_asteroid = asteroids[j]->get_pos().x;
      pos_y_asteroid = asteroids[j]->get_pos().y;

      if ( abs( pos_x_asteroid - pos_x_bullet ) < size_asteroid && abs( pos_y_asteroid - pos_y_bullet ) < size_asteroid){
        new_explosion(j);
        clean_asteroid(j);
        score++;
        std::string string_score = "Score : ";
        string_score += std::to_string(score);
        score_board.setString(string_score);
      }
    }
  }
  for (unsigned int j = 0; j < asteroids.size(); j++){
    pos_x_space_ship = space_ship.get_pos().x + sin(space_ship.get_rotation() * PI/180) * size_texture_space_ship.y/4;
    pos_y_space_ship = space_ship.get_pos().y - cos(space_ship.get_rotation() * PI/180) * size_texture_space_ship.y/4;

    pos_x_asteroid = asteroids[j]->get_pos().x;
    pos_y_asteroid = asteroids[j]->get_pos().y;

    if ( abs(pos_x_space_ship - pos_x_asteroid) < size_asteroid && abs(pos_y_space_ship - pos_y_asteroid) < size_asteroid ){
      new_explosion(j);
      clean_asteroid(j);
      space_ship.get_life() -= DAMAGE_ASTEROID;
      std::string string_life = "Vie : ";
      string_life += std::to_string(space_ship.get_life());
      life_board.setString(string_life);
      j--;
    }

    pos_x_space_ship = space_ship.get_pos().x - sin(space_ship.get_rotation() * PI/180) * size_texture_space_ship.y/4;
    pos_y_space_ship = space_ship.get_pos().y + cos(space_ship.get_rotation() * PI/180) * size_texture_space_ship.y/4;

    if ( abs(pos_x_space_ship - pos_x_asteroid) < size_asteroid && abs(pos_y_space_ship - pos_y_asteroid) < size_asteroid ){
      new_explosion(j);
      clean_asteroid(j);
      space_ship.get_life() -= DAMAGE_ASTEROID;
      std::string string_life = "Vie : ";
      string_life += std::to_string(space_ship.get_life());
      life_board.setString(string_life);
      j--;
    }
  }
}

void Game::clean_asteroid(unsigned int j){
  delete asteroids[j];
  asteroids.erase(begin(asteroids)+j);
}

void Game::new_explosion(unsigned int j){
  Explosion * newExplosion = new Explosion();
  newExplosion->set_position(asteroids[j]->get_pos());
  explosions.push_back(newExplosion);
}

void Game::test_game_over(void){
  if ( space_ship.get_life() <= 0 ){
    life_board.setString("GAME OVER");
    game_over = true;
  }
}

void Game::render(void){

  window.clear();
  window.draw(sprite_fond);
  window.draw(space_ship.get_sprite());
  unsigned int size_bullets = bullets.size();
  unsigned int size_asteroids = asteroids.size();

  for ( unsigned int i = 0; i < size_bullets; i++){
    window.draw(bullets[i]->get_sprite());
  }
  for ( unsigned int i = 0; i < size_asteroids; i++){
    window.draw(asteroids[i]->get_sprite());
  }
  for ( unsigned int i = 0; i < explosions.size(); i++){
    sprite_explosion[explosions[i]->get_frame()]->setPosition(explosions[i]->get_position());
    window.draw(*(sprite_explosion[explosions[i]->get_frame()]));
    explosions[i]->set_frame_plus_1();
    if ( explosions[i]->get_frame() == 48 ){
      clean_explosion(i);
    }
  }

  window.draw(score_board);
  window.draw(life_board);
  window.display();
}

void Game::check_nb_asteroid(void){
  if (asteroids.size() < NB_ASTEROIDS){
    new_asteroid();
  }

}

void Game::new_asteroid(void){
  Asteroid * newAsteroid = new Asteroid (ASTEROID_LIFE,SPEED_ASTEROID);
  sf::Vector2f position;
  float rotation = (float) rand_a_b(0,360);
  newAsteroid->get_sprite().setTexture(textures.texture_asteroid);
  newAsteroid->get_sprite().setScale(sf::Vector2f(SIZE_ASTEROID,SIZE_ASTEROID));
  sf::FloatRect spriteSize = newAsteroid->get_sprite().getLocalBounds();
  newAsteroid->get_sprite().setOrigin(spriteSize.width/2,spriteSize.height/2);
  position = get_rand_pos();
  newAsteroid->set_pos_abs(position.x,position.y);
  newAsteroid->set_rot(rotation);
  asteroids.push_back(newAsteroid);
}

sf::Vector2f Game::get_rand_pos(void){
  sf::Vector2f position(0.f, 0.f);
  int side = rand_a_b(1,4);
  switch(side){
    case 1 :
      position.x = (float) rand_a_b(0, SIZE_OF_WINDOW_X);
      position.y = 0.f;
      break;
    case 2 :
      position.x = (float) SIZE_OF_WINDOW_X;
      position.y = (float) rand_a_b(0, SIZE_OF_WINDOW_Y);
      break;
    case 3 :
      position.x = (float) rand_a_b(0, SIZE_OF_WINDOW_X);
      position.y = (float) SIZE_OF_WINDOW_Y;
      break;
    case 4 :
      position.x = 0.f;
      position.y = (float) rand_a_b(0, SIZE_OF_WINDOW_Y);
      break;
  }
  return position;
}

void Game::new_bullet(void){
  Bullet * newBullet = new Bullet (1,SPEED_BULLETS);
  newBullet->get_sprite().setTexture(textures.texture_bullet);
  newBullet->set_rotation_abs(space_ship.get_rotation());
  sf::Vector2f position = space_ship.get_pos();
  newBullet->set_pos_abs(position);
  newBullet->get_sprite().scale(sf::Vector2f(SIZE_BULLET,SIZE_BULLET));
  sf::FloatRect spriteSize = newBullet->get_sprite().getLocalBounds();
  newBullet->get_sprite().setOrigin(spriteSize.width/2,spriteSize.height/2);
  bullets.push_back(newBullet);
}

void Game::asteroids_move(void){
  for( unsigned int i = 0; i < get_asteroids().size(); i++){
    asteroids[i]->
    set_pos_rel(
      sin(asteroids[i]->get_rot()* PI / 180)*SPEED_ASTEROID,
      -SPEED_ASTEROID*cos(asteroids[i]->get_rot()* PI / 180));
    if_asteroid_is_out_of_screen(asteroids[i]);
    asteroids[i]->set_rotation_rel(SPEED_R_ASTEROIDS);
  }

}

void Game::bullets_move(void){
  for (unsigned int i = 0; i < get_bullets().size(); i++){
    bullets[i]->
    set_pos_rel(
      sin(bullets[i]->get_rotation()* PI / 180)*SPEED_BULLETS,
      -SPEED_BULLETS*cos(bullets[i]->get_rotation()* PI / 180));
   if_bullet_is_out_of_screen(bullets[i],i);
  }
}

Game::Game(int largeur, int hauteur, std::string name) :
  window(sf::VideoMode(largeur,hauteur), name),
  event(),
  space_ship(CD_TIR,LIFE_SPACE_SHIP,SPEED_SPACE_SHIP),
  font(),
  score_board(),
  life_board(),
  score(0),
  game_over(false) {
    textures.texture_fond.loadFromFile("./include/image/fond.jpg");
    textures.texture_bullet.loadFromFile("./include/image/Tire.png");
    textures.texture_space_ship.loadFromFile("./include/image/Vaisseau2.png");
    textures.texture_asteroid.loadFromFile("./include/image/asteroide.png");
    textures.texture_explosion.loadFromFile("./include/image/explosion.png");
    font.loadFromFile("./include/police/arial.ttf");
    window.setFramerateLimit(FRAMERATE_LIMIT);
    sprite_fond.setTexture(textures.texture_fond);
    initialisation_explosion();
    Initialisation_textures_size();
    Initialisation_space_ship();
    score_board.setFont(font);
    score_board.setString("Score : 0");
    score_board.setFillColor(sf::Color::White);
    score_board.setCharacterSize(SCORE_BOARD_SIZE);
    sf::FloatRect score_board_size = score_board.getLocalBounds();
    life_board.setFont(font);
    std::string string_life = "Vie : ";
    string_life += std::to_string(LIFE_SPACE_SHIP);
    life_board.setString(string_life);
    life_board.setFillColor(sf::Color::White);
    life_board.setCharacterSize(LIFE_BOARD_SIZE);
    life_board.setPosition(0.f, 2 * score_board_size.height);
}

void Game::Initialisation_textures_size(void){
  size_texture_asteroid = textures.texture_asteroid.getSize();
  size_texture_bullet = textures.texture_bullet.getSize();
  size_texture_space_ship = textures.texture_space_ship.getSize();

  size_texture_bullet.x *= SIZE_BULLET;
  size_texture_bullet.y *= SIZE_BULLET;
  size_texture_asteroid.x *= SIZE_ASTEROID;
  size_texture_asteroid.y *= SIZE_ASTEROID;
  size_texture_space_ship.x *= SIZE_SPACE_SHIP;
  size_texture_space_ship.y *= SIZE_SPACE_SHIP;
}

void Game::Initialisation_space_ship(void) {
  space_ship.get_sprite().setTexture(textures.texture_space_ship);
  space_ship.get_sprite().setScale(sf::Vector2f(SIZE_SPACE_SHIP,SIZE_SPACE_SHIP));
  sf::FloatRect spriteSize = space_ship.get_sprite().getLocalBounds();
  space_ship.get_sprite().setOrigin(spriteSize.width/2,spriteSize.height/2);
  space_ship.get_sprite().move(SIZE_OF_WINDOW_X/2,SIZE_OF_WINDOW_Y/2);
}

void Game::clean(void) {

  for ( unsigned int i = 0; i < bullets.size() ; i++ ){
    delete bullets[i];
  }
  for ( unsigned int i = 0; i < asteroids.size() ; i++ ){
    delete asteroids[i];
  }
  for ( unsigned int i = 0; i < explosions.size() ; i++ ){
    delete explosions[i];
  }
  for ( unsigned int i = 0; i < sprite_explosion.size() ; i++ ){
    delete sprite_explosion[i];
  }
  bullets.clear();
  asteroids.clear();
  explosions.clear();
  sprite_explosion.clear();
}

void Game::initialisation_explosion(void){
  for ( unsigned int i = 1; i <= 48; i++ ){
    sf::Sprite * frame = new sf::Sprite;
    frame->setTexture(textures.texture_explosion);
    frame->setTextureRect(sf::IntRect(256*(i-1),0,256,256));
    sf::FloatRect spriteSize = frame->getLocalBounds();
    frame->setOrigin(spriteSize.width/2,spriteSize.height/2);
    frame->setPosition(0,0);
    frame->scale(SIZE_EXPLOSION,SIZE_EXPLOSION);
    sprite_explosion.push_back(frame);
  }
}

void Game::final_explosion(void){
  static int frame = 0 ;
  if ( frame <= 500 ){
    if ( frame%20 == 0 ) space_ship.get_sprite().scale(0.01,0.01);
    else if ( frame%20 == 19 ) space_ship.get_sprite().scale(100,100);
  }
  else if ( frame == 501 ){
    Explosion * newExplosion = new Explosion();
    newExplosion->set_position(space_ship.get_pos());
    explosions.push_back(newExplosion);
    for ( unsigned int i = 0; i < 48; i++ ){
      sprite_explosion[i]->scale(20,20);
    }
  }
  else if ( frame == 600 ) window.close() ;
  frame++;
}
