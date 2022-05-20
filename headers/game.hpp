#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "boids.hpp"

class Game {
 private:
  sf::RenderWindow window_;
  Boids boids_;
  sf::Sprite background_;

 public:
  Game(std::string const& title, BoidsOptions const& boids_options,
       sf::Texture const& texture);
  void listen_for_event();
  void run();
};

#endif