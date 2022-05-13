#ifndef RULES_HPP
#define RULES_HPP

#include <cassert>
#include <iostream>
#include <numeric>

#include "../vectors/vectors.hpp"
#include "flock.hpp"

inline Vector2 apply_separation(std::vector<Bird> const& neighbors, Bird& bird,
                                double separation_distance, double separation) {
  Vector2 position_sum = std::accumulate(
      neighbors.begin(), neighbors.end(), Vector2{0, 0},
      [&](Vector2& sum, Bird const& neighbor) {
        bool are_close = (bird.position - neighbor.position).magnitude() <
                         separation_distance;

        return are_close ? sum + (bird.position - neighbor.position) : sum;
      });

  return position_sum * separation;
}

inline Vector2 apply_alignment(std::vector<Bird> const& neighbors, Bird& bird,
                               double alignment) {
  Vector2 velocity_sum =
      std::accumulate(neighbors.begin(), neighbors.end(), Vector2{0, 0},
                      [&](Vector2 const& velocity_sum, Bird const& neighbor) {
                        return (velocity_sum + neighbor.velocity);
                      });

  assert(neighbors.size() > 0);

  velocity_sum /= static_cast<double>(neighbors.size());

  return (velocity_sum - bird.velocity) * alignment;
}

inline Vector2 apply_cohesion(std::vector<Bird> const& neighbors, Bird& bird,
                              double cohesion) {
  Vector2 center_of_mass =
      std::accumulate(neighbors.begin(), neighbors.end(), Vector2{0, 0},
                      [&](Vector2 const& sum, Bird const& neighbor) {
                        return sum + neighbor.position;
                      });

  assert(neighbors.size() > 0);
  center_of_mass /= static_cast<double>(neighbors.size());

  return (center_of_mass - bird.position) * cohesion;
}

inline Vector2 avoid_predator(std::vector<Bird>& birds, Bird& bird, int index,
                              Bird const& predator, double separation_distance,
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

inline void avoid_boundaries(Bird& bird, int const canvas_width,
                             int const canvas_height) {
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
}

inline void avoid_speeding(Bird& bird, double max_speed = 5,
                           double min_speed = 2) {
  double speed = bird.velocity.magnitude();
  if (speed > max_speed) {
    bird.velocity /= speed;
    bird.velocity *= max_speed;
  } else if (speed < min_speed) {
    bird.velocity /= speed;
    bird.velocity *= min_speed;

    if (bird.velocity == Vector2{0, 0}) bird.velocity += Vector2{0, 2};
  }
}

#endif