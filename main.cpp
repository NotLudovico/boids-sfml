#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

#include "./flock/flock.hpp"
#include "./game/game.hpp"
#include "./ui/text.hpp"

void get_user_input(FlockOptions&);

int main() {
  FlockOptions flock_options;
  flock_options.canvas_height = sf::VideoMode::getDesktopMode().height * 0.9;
  flock_options.canvas_width = sf::VideoMode::getDesktopMode().width * 0.9;

  // get_user_input(flock_options);

  sf::Texture background;
  if (!background.loadFromFile("./ui/sky.jpg"))
    std::cout << "Failed to load background\n";

  Game game{"Boids Simulation", flock_options, background};
  game.run();

  return 0;
}

void get_user_input(FlockOptions& options) {
  std::cout << "Use default config? (Y/N)\n";
  char choice;
  std::cin >> choice;

  if (std::tolower(choice) != 'y') {
    std::cout << "Insert Number of Boids: ";
    std::cin >> options.number;

    std::cout << "Insert Separation Parameter: ";
    std::cin >> options.separation;

    std::cout << "Insert Alignment Parameter: ";
    std::cin >> options.alignment;

    std::cout << "Insert Cohesion Parameter: ";
    std::cin >> options.cohesion;

    std::cout << "Insert Distance: ";
    std::cin >> options.distance;

    std::cout << "Insert Separation Distance: ";
    std::cin >> options.separation_distance;

    std::cout << "Insert View Angle (0-3.14): ";
    std::cin >> options.view_angle;

    std::cout << "Simulate with predator? (Y/N): ";
    char choice;
    std::cin >> choice;

    if (std::tolower(choice) == 'y') options.with_predator = true;
  }
}