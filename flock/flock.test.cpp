#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "flock.hpp"

#include "../doctest.h"
#include "rules.hpp"

TEST_CASE("Testing rules") {
  Vector2 position_1{10, 14};
  Vector2 position_2{23, 20};
  Vector2 position_3{0, 0};
  Vector2 position_4{300, 300};
  Vector2 velocity_1{0, 0};
  Vector2 velocity_2{5, -4};
  Vector2 velocity_3{-3, -2};
  Vector2 velocity_4{2, 2};
  Bird b1{position_1, velocity_1};
  Bird b2{position_2, velocity_2};
  Bird b3{position_3, velocity_3};
  Bird b4{position_4, velocity_4};
  SUBCASE("Testing separation") {
    std::vector<Bird> flock_;
    flock_.push_back(b1);
    flock_.push_back(b2);
    flock_.push_back(b3);
    Vector2 v2 = apply_separation(flock_, b1, 30, 0.1);
    Vector2 v_s2{-0.3, 0.8};
    CHECK(v2 == v_s2);
  }

  SUBCASE("Testing alignement") {
    std::vector<Bird> flock_;
    flock_.push_back(b1);
    flock_.push_back(b2);
    flock_.push_back(b3);
    Vector2 v2 = apply_alignment(flock_, b1, 0.03);
    Vector2 v_a2{0.02, -0.06};
    CHECK(v2 == v_a2);
  }
  SUBCASE("Testing cohesion") {
    std::vector<Bird> flock_;
    flock_.push_back(b1);
    flock_.push_back(b2);
    flock_.push_back(b3);
    Vector2 v2 = apply_cohesion(flock_, b1, 0.005);
    Vector2 v_c2{0.005, -2.66666};
    CHECK(v2.x() == doctest::Approx(v_c2.x()));
  }
}
