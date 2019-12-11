#ifndef GAME_HPP
#define GAME_HPP

#include "declaration.hpp"
#include "Entity.hpp"
#include "Keys.hpp"
#include "Bullet.hpp"
#include "SpaceShip.hpp"
#include "Explosion.hpp"

class Game {
  private :
    sf::RenderWindow window;
    sf::Event event;
    sf::Sprite sprite_fond;
    std::vector<sf::Sprite *> sprite_explosion;
    std::vector<Explosion *> explosions;
    Texture_Group textures;
    SpaceShip space_ship;
    Keys keys;
    sf::Vector2u size_texture_asteroid;
    sf::Vector2u size_texture_bullet;
    sf::Vector2u size_texture_space_ship;
    std::vector<Bullet *> bullets;
    std::vector<Asteroid *> asteroids;
    sf::Font font;
    sf::Text score_board;
    sf::Text life_board;
    unsigned int score;
    bool game_over;
  public :
    Game(int largeur, int hauteur, std::string name);
    void run(void);
    void ProcessEvents(void);
    void update(void);
    void render(void);
    void check_key_pressed(void);
    void check_key_released(void);
    void key_move(void);
    void if_space_ship_is_out_screen(void);
    void new_bullet(void);
    void bullets_move(void);
    void check_nb_asteroid(void);
    void new_asteroid(void);
    void new_explosion(unsigned int j);
    void asteroids_move(void);
    std::vector<Bullet *> get_bullets(void) {return bullets;}
    std::vector<Asteroid *> get_asteroids(void) {return asteroids;}
    void if_bullet_is_out_of_screen(Bullet * bullet, int i);
    void if_asteroid_is_out_of_screen(Asteroid * asteroid);
    SpaceShip & get_space_ship(void) {return space_ship;}
    void Initialisation_space_ship(void);
    void Initialisation_textures_size(void);
    void test_collisions(void);
    sf::Vector2f get_rand_pos(void);
    void test_game_over(void);
    void clean_asteroid(unsigned int j);
    void clean(void);
    void initialisation_explosion(void);
    void clean_explosion(unsigned int i);
    void final_explosion(void);
};

#endif
