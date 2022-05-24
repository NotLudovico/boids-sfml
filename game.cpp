#include "./headers/game.hpp"

#include <iostream>

#include "./headers/boids.hpp"
#include "./headers/text.hpp"

Game::Game(std::string const& title, BoidsOptions const& boids_options,
           sf::Texture const& texture)
    : window_{sf::VideoMode(boids_options.canvas_width,
                            boids_options.canvas_height),
              title},
      boids_{boids_options},
      background_{texture} {
  window_.setFramerateLimit(30);
  window_.setVerticalSyncEnabled(true);

  sf::Vector2u texture_size = texture.getSize();
  sf::Vector2u window_size = window_.getSize();

  float scale_x = static_cast<float>(window_size.x) / texture_size.x;
  float scale_y = static_cast<float>(window_size.y) / texture_size.y;

  background_.setTexture(texture);
  background_.setScale(scale_x, scale_y);
}

void Game::listen_for_event() {
  sf::Event event;
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed ||
        (event.type == sf::Event::KeyPressed &&
         event.key.code == sf::Keyboard::Escape))
      window_.close();
  }
}

void Game::run() {
  sf::Font font;

  bool has_loaded_font = true;
  if (!font.loadFromFile("./ui/catamaran.ttf")) {
    std::cerr << "Couldn't load font";
    has_loaded_font = false;
  }

  Text mean_velocity_text{"", 32, sf::Color::Black, sf::Vector2f{10.f, 10.f},
                          font};
  Text standard_deviation_text{"", 32, sf::Color::Black,
                               sf::Vector2f{10.f, 120.f}, font};
  Text alive_counter{"", 32, sf::Color::Black, sf::Vector2f{10.f, 220.f}, font};

  while (window_.isOpen()) {
    this->listen_for_event();

    window_.clear(sf::Color::White);
    Statistic stats = boids_.calculate_statistics();
    window_.draw(background_);
    boids_.evolve();
    boids_.draw(window_);

    if (has_loaded_font) {
      mean_velocity_text.update_text(
          "MEAN VELOCITY X: " + std::to_string(stats.mean_velocity.x()) +
          "\nMEAN VELOCITY Y: " + std::to_string(stats.mean_velocity.y()));

      standard_deviation_text.update_text(
          "STANDARD DEVIATION VELOCITY X: " +
          std::to_string(stats.stdev_velocity.x()) +
          "\nSTANDARD DEVIATION VELOCITY Y: " +
          std::to_string(stats.stdev_velocity.y()));

      alive_counter.update_text("BOIDS ALIVE: " +
                                std::to_string(boids_.size()));
      mean_velocity_text.draw(window_);
      standard_deviation_text.draw(window_);
      alive_counter.draw(window_);
    }

    window_.display();
  }
}