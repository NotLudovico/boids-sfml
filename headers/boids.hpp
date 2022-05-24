#ifndef FLOCK_HPP
#define FLOCK_HPP

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>

#include "vectors.hpp"

struct Boid {
  Vector2 position{0, 0};
  Vector2 velocity{0, 0};

  double x() const { return position.x(); }
  double y() const { return position.y(); }
  double vx() const { return velocity.x(); }
  double vy() const { return velocity.y(); }
};

enum SpaceType { cilindrical, toroidal, rectangular };

struct BoidsOptions {
  int number = 1000;
  double separation = 0.05;
  double alignment = 0.02;
  double cohesion = 0.005;
  double distance = 150;
  double separation_distance = 35;
  double view_angle = 2.8;  // ~ 320
  bool with_predator = false;
  unsigned int canvas_height = 1000;
  unsigned int canvas_width = 1000;
  SpaceType space = toroidal;
};

struct Statistic {
  Vector2 mean_velocity;
  Vector2 stdev_velocity;
};

class Boids {
 private:
  double separation_;
  double alignment_;
  double cohesion_;
  double distance_;
  double separation_distance_;
  std::vector<Boid> boids_;
  Boid predator_;
  bool with_predator_;
  double view_angle_;
  unsigned int canvas_height_;
  unsigned int canvas_width_;
  SpaceType space_;

 public:
  Boids(BoidsOptions const& boids_options);
  void evolve();
  void evolve_predator();
  void draw(sf::RenderWindow& window) const;
  unsigned int size() const;
  Statistic calculate_statistics() const;
};

#endif