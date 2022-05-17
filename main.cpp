#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

#include "./headers/boids.hpp"
#include "./headers/game.hpp"
#include "./headers/text.hpp"

void get_user_input(BoidsOptions&);

int main() {
  BoidsOptions options;
  options.canvas_height = sf::VideoMode::getDesktopMode().height * 0.9;
  options.canvas_width = sf::VideoMode::getDesktopMode().width * 0.9;

  std::cout << "Use default config? (Y/N)\n";
  char choice;
  std::cin >> choice;

  try {
    if (!std::cin.good()) throw std::runtime_error{"Invalid choice"};
    if (std::tolower(choice) != 'y') {
      std::cout << "Insert Number of Boids: ";
      std::cin >> options.number;

      if (options.number <= 0)
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

      if (options.view_angle < 0)
        throw std::runtime_error{"Invalid angle value"};

      std::cout << "Simulate with predator? (Y/N): ";
      char choice;
      std::cin >> choice;

      std::cout << "Select Space Type: \n (0) - Cilindrical \n (1) - Toroidal "
                   "\n (2) - Rectangular\n";
      int space_type;
      std::cin >> space_type;

      if (space_type < 0 || space_type > 2)
        throw std::runtime_error{"Invalid space type"};

      switch (space_type) {
        case 1:
          options.space = toroidal;
          break;
        case 2:
          options.space = rectangular;
          break;
      }

      if (std::tolower(choice) == 'y') options.with_predator = true;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  remove("data.csv");

  sf::Texture background;
  if (!background.loadFromFile("./ui/sky.jpg"))
    std::cout << "Failed to load background\n";

  Game game{"Boids Simulation", options, background};
  game.run();

  return 0;
}
