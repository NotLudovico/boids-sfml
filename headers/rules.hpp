#ifndef RULES_HPP
#define RULES_HPP

#include <cassert>
#include <iostream>
#include <numeric>

#include "boids.hpp"
#include "vectors.hpp"

inline std::vector<Boid> get_neighbors(std::vector<Boid> const& boids,
                                       Boid const& boid, double distance,
                                       double view_angle) {
  std::vector<Boid> neighbors;

  std::for_each(boids.begin(), boids.end(), [&](Boid const& other) {
    const double distance_between_boids =
        (other.position - boid.position).magnitude();

    if (distance_between_boids < distance && distance_between_boids > 0) {
      double angle = get_angle(other.position - boid.position, boid.velocity);

      if (angle < view_angle) neighbors.push_back(other);
    }
  });

  return neighbors;
}

inline Vector2 apply_separation(std::vector<Boid> const& neighbors, Boid& boid,
                                double separation_distance, double separation) {
  if (neighbors.size() == 0) return Vector2{0, 0};

  Vector2 position_sum = std::accumulate(
      neighbors.begin(), neighbors.end(), Vector2{0, 0},
      [&](Vector2& sum, Boid const& neighbor) {
        bool are_close = (boid.position - neighbor.position).magnitude() <
                         separation_distance;

        return are_close ? sum + (boid.position - neighbor.position) : sum;
      });

  return position_sum * separation;
}

inline Vector2 apply_alignment(std::vector<Boid> const& neighbors, Boid& boid,
                               double alignment) {
  if (neighbors.size() == 0) return Vector2{0, 0};

  Vector2 velocity_sum =
      std::accumulate(neighbors.begin(), neighbors.end(), Vector2{0, 0},
                      [&](Vector2 const& velocity_sum, Boid const& neighbor) {
                        return (velocity_sum + neighbor.velocity);
                      });

  velocity_sum /= static_cast<double>(neighbors.size());

  return (velocity_sum - boid.velocity) * alignment;
}

inline Vector2 apply_cohesion(std::vector<Boid> const& neighbors, Boid& boid,
                              double cohesion) {
  if (neighbors.size() == 0) return Vector2{0, 0};

  Vector2 center_of_mass =
      std::accumulate(neighbors.begin(), neighbors.end(), Vector2{0, 0},
                      [&](Vector2 const& sum, Boid const& neighbor) {
                        return sum + neighbor.position;
                      });

  center_of_mass /= static_cast<double>(neighbors.size());

  return (center_of_mass - boid.position) * cohesion;
}

inline Vector2 avoid_predator(Boid& boid, int index,
                              std::vector<int>& dead_boid_indexes,
                              Boid const& predator, double separation_distance,
                              double view_angle) {
  Vector2 position_sum{0, 0};

  if ((boid.position - predator.position).magnitude() < 8) {
    dead_boid_indexes.push_back(index);
    return position_sum;
  }

  bool const are_close =
      (boid.position - predator.position).magnitude() < separation_distance;
  double angle = get_angle(predator.position - boid.position, boid.velocity);

  if (are_close && angle < view_angle) {
    position_sum += (boid.position - predator.position) * 5;
  }

  return position_sum;
}

inline void avoid_boundaries(Boid& boid, int const canvas_width,
                             int const canvas_height, SpaceType space) {
  if (space == cilindrical) {
    if (boid.x() < 0) {
      boid.position.set_x(canvas_width);
    } else if (boid.x() > canvas_width) {
      boid.position.set_x(0);
    }

    if (boid.y() < 100) {
      boid.velocity += Vector2{0, 0.5};
    } else if (boid.y() > (canvas_height - 100)) {
      boid.velocity += Vector2{0, -0.5};
    }
  } else if (space == toroidal) {
    if (boid.x() < 0) {
      boid.position.set_x(canvas_width);
    } else if (boid.x() > canvas_width) {
      boid.position.set_x(0);
    }

    if (boid.y() < 0) {
      boid.position.set_y(canvas_height);
    } else if (boid.y() > canvas_height) {
      boid.position.set_y(0);
    }
  } else {
    if (boid.x() < 100) {
      boid.velocity += Vector2{0.5, 0};
    } else if (boid.x() > (canvas_width - 200)) {
      boid.velocity += Vector2{-0.5, 0};
    }

    if (boid.y() < 100) {
      boid.velocity += Vector2{0, 0.5};
    } else if (boid.y() > (canvas_height - 200)) {
      boid.velocity += Vector2{0, -0.5};
    }
  }
}

inline void avoid_speeding(Boid& boid, double max_speed = 5,
                           double min_speed = 2) {
  if (boid.velocity == Vector2{0, 0}) boid.velocity += Vector2{0, 2};
  double speed = boid.velocity.magnitude();

  if (speed > max_speed) {
    boid.velocity /= speed;
    boid.velocity *= max_speed;
  } else if (speed < min_speed) {
    boid.velocity /= speed;
    boid.velocity *= min_speed;
  }
}

#endif