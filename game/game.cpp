#include "./game.hpp"

#include <iostream>

#include "../flock/flock.hpp"
#include "../ui/text.hpp"

Game::Game(std::string const& title, FlockOptions const& flock_options,
           sf::Texture const& texture)
    : window_{sf::VideoMode(flock_options.canvas_width,
                            flock_options.canvas_height),
              title},
      flock_{flock_options},
      background_{texture} {
  window_.setVerticalSyncEnabled(true);
  window_.setFramerateLimit(30);

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
  if (!font.loadFromFile("./ui/catamaran.ttf"))
    std::cerr << "Couldn't load font";

  Text m_velocity{"", 32, sf::Color::Black, sf::Vector2f{10.f, 10.f}, font};
  Text s_velocity{"", 32, sf::Color::Black, sf::Vector2f{10.f, 120.f}, font};
  Text alive_counter{"", 32, sf::Color::Black, sf::Vector2f{10.f, 220.f}, font};

  while (window_.isOpen()) {
    this->listen_for_event();

    window_.clear(sf::Color::White);
    Statistic m_vel = flock_.calculate_statistics();

    m_velocity.update_text(
        "MEAN VELOCITY X: " + std::to_string(m_vel.mean_velocity.x()) +
        "\nMEAN VELOCITY Y: " + std::to_string(m_vel.mean_velocity.y()));

    s_velocity.update_text("STANDARD DEVIATION VELOCITY X: " +
                           std::to_string(m_vel.stdev_velocity.x()) +
                           "\nSTANDARD DEVIATION VELOCITY Y: " +
                           std::to_string(m_vel.stdev_velocity.y()));

    alive_counter.update_text("BOIDS ALIVE: " + std::to_string(flock_.size()));

    window_.draw(background_);
    flock_.evolve();
    flock_.draw(window_);
    m_velocity.draw(window_);
    s_velocity.draw(window_);
    alive_counter.draw(window_);

    window_.display();
  }
}