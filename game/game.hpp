#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "../flock/flock.hpp"

class Game {
 private:
  sf::RenderWindow window_;
  Flock flock_;
  sf::Sprite background_;

 public:
  Game(std::string const& title, FlockOptions const& flock_options,
       sf::Texture const& texture);
  void listen_for_event();
  void run();
};

#endif