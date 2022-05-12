#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

#include "./flock/flock.hpp"
#include "./game/game.hpp"
#include "./ui/text.hpp"

void get_user_input(FlockOptions&);

int main() {
  FlockOptions options;
  options.canvas_height = sf::VideoMode::getDesktopMode().height * 0.9;
  options.canvas_width = sf::VideoMode::getDesktopMode().width * 0.9;

  // get_user_input(flock_options);
  std::cout << "Use default config? (Y/N)\n";
  char choice;
  std::cin >> choice;

  try {
    if (std::tolower(choice) != 'y') {
      std::cout << "Insert Number of Boids: ";
      std::cin >> options.number;

      if (options.number < 0)
        throw std::runtime_error{"Number of boids is invalid"};

      std::cout << "Insert Separation Parameter: ";
      std::cin >> options.separation;

      if (options.separation < 0)
        throw std::runtime_error{"Separation coefficient is invalid"};

      std::cout << "Insert Alignment Parameter: ";
      std::cin >> options.alignment;

      if (options.alignment < 0)
        throw std::runtime_error{"Alignment coefficient is invalid"};

      std::cout << "Insert Cohesion Parameter: ";
      std::cin >> options.cohesion;

      if (options.cohesion < 0)
        throw std::runtime_error{"Cohesion coefficient is invalid"};

      std::cout << "Insert Distance: ";
      std::cin >> options.distance;

      if (options.distance < 0) throw std::runtime_error{"Distance is invalid"};

      std::cout << "Insert Separation Distance: ";
      std::cin >> options.separation_distance;

      if (options.separation_distance < 0)
        throw std::runtime_error{"Separation Distance is invalid"};

      std::cout << "Insert View Angle (0-3.14): ";
      std::cin >> options.view_angle;

      std::cout << "Simulate with predator? (Y/N): ";
      char choice;
      std::cin >> choice;

      if (std::tolower(choice) == 'y') options.with_predator = true;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  sf::Texture background;
  if (!background.loadFromFile("./ui/sky.jpg"))
    std::cout << "Failed to load background\n";

  Game game{"Boids Simulation", options, background};
  game.run();

  return 0;
}

void get_user_input(FlockOptions& options) {}