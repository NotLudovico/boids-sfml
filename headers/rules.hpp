#ifndef RULES_HPP
#define RULES_HPP

#include <cassert>
#include <iostream>
#include <numeric>

#include "boids.hpp"
#include "vectors.hpp"

inline std::vector<Boid> get_neighbors(std::vector<Boid> const& birds,
                                       Boid const& bird, double distance,
                                       double view_angle) {
  std::vector<Boid> neighbors;

  std::for_each(birds.begin(), birds.end(), [&](Boid const& other) {
    const double distance_between_birds =
        (other.position - bird.position).magnitude();

    if (distance_between_birds < distance && distance_between_birds > 0) {
      double angle = get_angle(other.position - bird.position, bird.velocity);

      if (angle < view_angle) neighbors.push_back(other);
    }
  });

  return neighbors;
}

inline Vector2 apply_separation(std::vector<Boid> const& neighbors, Boid& bird,
                                double separation_distance, double separation) {
  Vector2 position_sum = std::accumulate(
      neighbors.begin(), neighbors.end(), Vector2{0, 0},
      [&](Vector2& sum, Boid const& neighbor) {
        bool are_close = (bird.position - neighbor.position).magnitude() <
                         separation_distance;

        return are_close ? sum + (bird.position - neighbor.position) : sum;
      });

  return position_sum * separation;
}

inline Vector2 apply_alignment(std::vector<Boid> const& neighbors, Boid& bird,
                               double alignment) {
  Vector2 velocity_sum =
      std::accumulate(neighbors.begin(), neighbors.end(), Vector2{0, 0},
                      [&](Vector2 const& velocity_sum, Boid const& neighbor) {
                        return (velocity_sum + neighbor.velocity);
                      });

  assert(neighbors.size() > 0);

  velocity_sum /= static_cast<double>(neighbors.size());

  return (velocity_sum - bird.velocity) * alignment;
}

inline Vector2 apply_cohesion(std::vector<Boid> const& neighbors, Boid& bird,
                              double cohesion) {
  Vector2 center_of_mass =
      std::accumulate(neighbors.begin(), neighbors.end(), Vector2{0, 0},
                      [&](Vector2 const& sum, Boid const& neighbor) {
                        return sum + neighbor.position;
                      });

  assert(neighbors.size() > 0);
  center_of_mass /= static_cast<double>(neighbors.size());

  return (center_of_mass - bird.position) * cohesion;
}

inline Vector2 avoid_predator(std::vector<Boid>& birds, Boid& bird, int index,
                              Boid const& predator, double separation_distance,
                              double view_angle) {
  Vector2 position_sum{0, 0};

  if ((bird.position - predator.position).magnitude() < 8) {
    birds.erase(birds.begin() + index);
    return position_sum;
  }

  bool const are_close =
      (bird.position - predator.position).magnitude() < separation_distance;
  double angle = get_angle(predator.position - bird.position, bird.velocity);

  if (are_close && angle < view_angle) {
    position_sum += (bird.position - predator.position) * 5;
  }

  return position_sum;
}

inline void avoid_boundaries(Boid& bird, int const canvas_width,
                             int const canvas_height, SpaceType space) {
  if (space == cilindrical) {
    if (bird.x() < 0) {
      bird.position.set_x(canvas_width);
    } else if (bird.x() > canvas_width) {
      bird.position.set_x(0);
    }

    if (bird.y() < 100) {
      bird.velocity += Vector2{0, 0.5};
    } else if (bird.y() > (canvas_height - 200)) {
      bird.velocity += Vector2{0, -0.5};
    }
  } else if (space == toroidal) {
    if (bird.x() < 0) {
      bird.position.set_x(canvas_width);
    } else if (bird.x() > canvas_width) {
      bird.position.set_x(0);
    }

    if (bird.y() < 0) {
      bird.position.set_y(canvas_height);
    } else if (bird.y() > canvas_height) {
      bird.position.set_y(0);
    }
  } else {
    if (bird.x() < 100) {
      bird.velocity += Vector2{0.5, 0};
    } else if (bird.x() > (canvas_width - 200)) {
      bird.velocity += Vector2{-0.5, 0};
    }

    if (bird.y() < 100) {
      bird.velocity += Vector2{0, 0.5};
    } else if (bird.y() > (canvas_height - 200)) {
      bird.velocity += Vector2{0, -0.5};
    }
  }
}

inline void avoid_speeding(Boid& bird, double max_speed = 5,
                           double min_speed = 2) {
  if (bird.velocity == Vector2{0, 0}) bird.velocity += Vector2{0, 2};

  double speed = bird.velocity.magnitude();

  if (speed > max_speed) {
    bird.velocity /= speed;
    bird.velocity *= max_speed;
  } else if (speed < min_speed) {
    bird.velocity /= speed;
    bird.velocity *= min_speed;
  }
}

#endif