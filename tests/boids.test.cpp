#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../headers/boids.hpp"
#include "../headers/rules.hpp"
#include "doctest.h"

TEST_CASE("Testing rules") {
  Boid b1{{10, 14}, {0, 0}};
  Boid b2{{23, 20}, {5, -4}};
  Boid b3{{0, 0}, {-3, -2}};
  Boid b4{{300, 300}, {2, 2}};

  SUBCASE("Testing get_neighbors") {
    std::vector<Boid> flock_;
    flock_.push_back(b1);
    flock_.push_back(b2);
    flock_.push_back(b3);
    flock_.push_back(b4);
    auto v = get_neighbors(flock_, b1, 150, 2.8);
    std::vector<Boid> flock_2;
    flock_2.push_back(b1);
    flock_2.push_back(b4);
    auto v2 = get_neighbors(flock_2, b4, 150, 2.8);
    std::vector<Boid> flock_3;
    flock_3.push_back(b1);
    auto v3 = get_neighbors(flock_3, b1, 150, 2.8);
    std::vector<Boid> flock_4;
    flock_.push_back(b1);
    flock_.push_back(b2);
    flock_.push_back(b4);
    auto v4 = get_neighbors(flock_4, b1, 150, 0.5);
    CHECK(v.size() == 2);
    CHECK(v2.size() == 0);
    CHECK(v3.size() == 0);
    CHECK(v4.size() == 0);
  }

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
