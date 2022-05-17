#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../headers/boids.hpp"
#include "../headers/rules.hpp"
#include "doctest.h"

TEST_CASE("Testing rules") {
  Boid b1{{10, 14}, {0, 0}};
  Boid b2{{23, 20}, {5, -4}};
  Boid b3{{0, 0}, {-3, -2}};
  Boid b4{{300, 300}, {2, 2}};
  SUBCASE("Testing separation") {
    std::vector<Boid> flock_;
    flock_.push_back(b1);
    flock_.push_back(b2);
    flock_.push_back(b3);
    Vector2 v2 = apply_separation(flock_, b1, 30, 0.1);
    Vector2 v_s2{-0.3, 0.8};
    CHECK(v2 == v_s2);
  }

  SUBCASE("Testing alignement") {
    std::vector<Boid> flock_;
    flock_.push_back(b1);
    flock_.push_back(b2);
    flock_.push_back(b3);
    Vector2 v2 = apply_alignment(flock_, b1, 0.03);
    Vector2 v_a2{0.02, -0.06};
    CHECK(v2 == v_a2);
  }
  SUBCASE("Testing cohesion") {
    std::vector<Boid> flock_;
    flock_.push_back(b1);
    flock_.push_back(b2);
    flock_.push_back(b3);
    Vector2 v2 = apply_cohesion(flock_, b1, 0.005);
    Vector2 v_c2{0.005, -2.66666};
    CHECK(v2.x() == doctest::Approx(v_c2.x()));
  }
}
