#include "./headers/boids.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>

#include "./headers/rules.hpp"
#include "./headers/vectors.hpp"

Boids::Boids(BoidsOptions const& boids_options)
    : separation_{boids_options.separation},
      alignment_{boids_options.alignment},
      cohesion_{boids_options.cohesion},
      distance_{boids_options.distance},
      separation_distance_{boids_options.separation_distance},
      with_predator_{boids_options.with_predator},
      view_angle_{boids_options.view_angle},
      canvas_height_{boids_options.canvas_height},
      canvas_width_{boids_options.canvas_width},
      space_{boids_options.space} {
  auto [number, separation, alignment, cohesion, distance, separation_distance,
        with_predator, view_angle, canvas_height, canvas_width, space] =
      boids_options;

  assert(number > 0 && separation >= 0 && alignment >= 0 && cohesion >= 0 &&
         distance >= 0 && separation_distance >= 0);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> pos_x(0, canvas_width);
  std::uniform_real_distribution<float> pos_y(0, canvas_height);
  std::uniform_real_distribution<float> vel(-5, 5);

  boids_.reserve(number);

  std::generate_n(back_inserter(boids_), number, [&]() {
    return Boid{{pos_x(gen), pos_y(gen)}, Vector2{vel(gen), vel(gen)}};
  });

  if (with_predator)
    predator_ =
        Boid{Vector2{pos_x(gen), pos_y(gen)}, Vector2{vel(gen), vel(gen)}};
}

void Boids::evolve_predator() {
  if (!with_predator_) return;

  std::vector<Boid> const neighbors =
      get_neighbors(boids_, predator_, distance_, view_angle_);

  if (neighbors.size() != 0) {
    predator_.velocity += apply_cohesion(neighbors, predator_, cohesion_ * 2);
    predator_.velocity += apply_alignment(neighbors, predator_, 0.01);
  }

  avoid_speeding(predator_, 15, 2);
  avoid_boundaries(predator_, canvas_width_, canvas_height_, space_);

  predator_.position += predator_.velocity;
}

void Boids::evolve() {
  evolve_predator();
  int counter = 0;
  std::for_each(boids_.begin(), boids_.end(), [&](Boid& bird) {
    std::vector<Boid> const neighbors =
        get_neighbors(boids_, bird, distance_, view_angle_);

    if (neighbors.size() != 0) {
      bird.velocity += (apply_separation(neighbors, bird, separation_distance_,
                                         separation_));
      bird.velocity += (apply_alignment(neighbors, bird, alignment_));
      bird.velocity += (apply_cohesion(neighbors, bird, cohesion_));
    }

    if (with_predator_) {
      bird.velocity += avoid_predator(boids_, bird, counter, predator_,
                                      separation_distance_, view_angle_);
    }
    avoid_speeding(bird);
    avoid_boundaries(bird, canvas_width_, canvas_height_, space_);

    bird.position += bird.velocity;
    ++counter;
  });
}

void Boids::draw(sf::RenderWindow& window) const {
  sf::CircleShape tri(6, 3);
  tri.setFillColor(sf::Color::Black);
  tri.setOrigin(sf::Vector2f{3, 3});

  for (size_t i = 0; i != boids_.size(); i++) {
    Boid const& bird = boids_[i];
    double angle = get_angle(bird.velocity, Vector2{0, 1}) * 180 / (3.1415 * 5);

    if (bird.vx() < 0 && bird.vy() < 0) angle -= 180;
    if (bird.vx() < 0 && bird.vy() > 0) angle = -90 - angle;
    if (bird.vx() > 0 && bird.vy() < 0) angle = 180 - angle;

    tri.setRotation(angle);
    tri.setPosition(sf::Vector2f(boids_[i].x(), boids_[i].y()));
    window.draw(tri);
  }

  if (with_predator_) {
    sf::CircleShape bird(8, 3);
    double angle =
        get_angle(predator_.velocity, Vector2{0, 1}) * 180 / (3.1415);
    if (predator_.vx() < 0 && predator_.vy() < 0) angle -= 180;
    if (predator_.vx() < 0 && predator_.vy() > 0) angle = -90 - angle;
    if (predator_.vx() > 0 && predator_.vy() < 0) angle = 180 - angle;
    bird.setFillColor(sf::Color::Red);
    bird.setOrigin(sf::Vector2f{4, 4});
    bird.setRotation(angle);
    bird.setPosition(sf::Vector2f(predator_.x(), predator_.y()));
    window.draw(bird);
  }
}

unsigned int Boids::size() const { return boids_.size(); }

Statistic Boids::calculate_statistics() const {
  if (boids_.size() != 0) {
    Vector2 mean_velocity =
        std::accumulate(boids_.begin(), boids_.end(), Vector2{0, 0},
                        [&](Vector2 const& sum, Boid const& bird) {
                          return (sum + bird.velocity);
                        });

    mean_velocity /= static_cast<float>(boids_.size());

    float stdev_x = 0;
    float stdev_y = 0;

    std::for_each(boids_.begin(), boids_.end(), [&](Boid const& bird) {
      stdev_x += (bird.velocity.x() - mean_velocity.x()) *
                 (bird.velocity.x() - mean_velocity.x());
      stdev_y += (bird.velocity.y() - mean_velocity.y()) *
                 (bird.velocity.y() - mean_velocity.y());
    });

    stdev_x /= static_cast<float>(boids_.size());
    stdev_y /= static_cast<float>(boids_.size());

    stdev_x = std::sqrt(stdev_x);
    stdev_y = std::sqrt(stdev_y);

    std::ofstream file;
    file.open("data.csv", std::ios::app);
    file << mean_velocity.x() << "," << mean_velocity.y() << " , " << stdev_x
         << " , " << stdev_y << "\n";
    file.close();

    return Statistic{mean_velocity, Vector2{stdev_x, stdev_y}};
  } else {
    return Statistic{{0, 0}, {0, 0}};
  }
}
