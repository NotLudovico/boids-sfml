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
  SUBCASE("Avoid speeding") {
    Boid b5{{300, 300}, {0, 0}};
    Boid b6{{300, 300}, {100, 100}};
    avoid_speeding(b5, 5, 2);
    avoid_speeding(b6, 5, 2);
    CHECK(b5.velocity.magnitude() == 2);
    CHECK(b6.velocity.magnitude() == 5);
  }
  SUBCASE("Avoid boundaries cilindrical") {
    Boid b7{{300, 950}, {0, 3}};
    Boid b8{{300, 50}, {0, -3}};
    Boid b9{{-20, 500}, {0, 0}};
    Boid b10{{1080, 500}, {5, 0}};
    for (size_t i = 0; i != 6; i++) {
      avoid_boundaries(b7, 1000, 1000, cilindrical);
    }
    CHECK(b7.vy() == 0);
    for (size_t i = 0; i != 6; i++) {
      avoid_boundaries(b8, 1000, 1000, cilindrical);
    }
    CHECK(b8.vy() == 0);
    avoid_boundaries(b9, 1000, 1000, cilindrical);
    CHECK(b9.x() == 1000);
    avoid_boundaries(b10, 1000, 1000, cilindrical);
    CHECK(b10.x() == 0);
  }
  SUBCASE("Avoid boundaries rectangular") {
    Boid b7{{300, 950}, {0, 3}};
    Boid b8{{300, 50}, {0, -3}};
    Boid b9{{-20, 500}, {0, 0}};
    Boid b10{{1080, 500}, {5, 0}};
    for (size_t i = 0; i != 6; i++) {
      avoid_boundaries(b7, 1000, 1000, rectangular);
    }
    CHECK(b7.vy() == 0);
    for (size_t i = 0; i != 6; i++) {
      avoid_boundaries(b8, 1000, 1000, rectangular);
    }
    CHECK(b8.vy() == 0);
    for (size_t i = 0; i != 6; i++) {
      avoid_boundaries(b9, 1000, 1000, rectangular);
    }
    CHECK(b9.vx() == 3);
    for (size_t i = 0; i != 6; i++) {
      avoid_boundaries(b10, 1000, 1000, rectangular);
    }
    CHECK(b10.vx() == 2);
  }
  SUBCASE("Avoid boundaries toroidal") {
    Boid b9{{-20, 500}, {0, 0}};
    Boid b10{{1080, 500}, {5, 0}};
    Boid b11{{500, -20}, {5, 0}};
    Boid b12{{500, 1020}, {5, 0}};
    avoid_boundaries(b9, 1000, 1000, toroidal);
    CHECK(b9.x() == 1000);
    avoid_boundaries(b10, 1000, 1000, toroidal);
    CHECK(b10.x() == 0);
    avoid_boundaries(b11, 1000, 1000, toroidal);
    CHECK(b11.y() == 1000);
    avoid_boundaries(b12, 1000, 1000, toroidal);
    CHECK(b12.y() == 0);
  }
}