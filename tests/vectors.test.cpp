#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../headers/vectors.hpp"

#include "doctest.h"

TEST_CASE("Testing 2D Vector") {
  SUBCASE("testing getters") {
    Vector2 v1{1., 2.};
    Vector2 v2{2., 3.};
    Vector2 v3{4., 3.};
    CHECK(v1.x() == 1.);
    CHECK(v1.y() == 2.);
    CHECK(v2.x() == 2.);
    CHECK(v3.y() == 3.);
  }
  SUBCASE("testing +") {
    Vector2 v1{1., 2.};
    Vector2 v2{2., 3.};
    Vector2 v3{4., 3.};
    Vector2 res1{3., 5.};
    Vector2 res2{6., 6.};
    Vector2 res3{5., 5.};
    CHECK(v1 + v2 == res1);
    CHECK(v2 + v3 == res2);
    CHECK(v1 + v3 == res3);
  }
  SUBCASE("testing -") {
    Vector2 v1{1., 2.};
    Vector2 v2{2., 3.};
    Vector2 v3{4., 3.};
    Vector2 res1{-1., -1.};
    Vector2 res2{-2., 0.};
    Vector2 res3{-3., -1.};
    CHECK(v1 - v2 == res1);
    CHECK(v2 - v3 == res2);
    CHECK(v1 - v3 == res3);
  }
  SUBCASE("testing *") {
    Vector2 v1{1., 2.};
    Vector2 v2{2., 3.};
    Vector2 v3{4., 3.};
    Vector2 res1{2., 4.};
    Vector2 res3{3., 6.};
    CHECK(v1 * 2. == res1);
    CHECK(v2 * 1. == v2);
    CHECK(v1 * 3. == res3);
  }
  SUBCASE("testing *") {
    Vector2 v1{1., 2.};
    Vector2 v2{2., 3.};
    Vector2 v3{4., 3.};
    CHECK(v1 * v2 == 8.);
    CHECK(v2 * v3 == 17.);
    CHECK(v1 * v3 == 10.);
  }
  SUBCASE("testing /") {
    Vector2 v1{1., 2.};
    Vector2 v2{2., 3.};
    Vector2 v3{4., 3.};
    Vector2 res1{1., 1.5};
    Vector2 res2{-4., -3.};
    CHECK(v1 / 1. == v1);
    CHECK(v2 / 2. == res1);
    CHECK(v3 / -1. == res2);
    CHECK_THROWS_WITH_AS(v1 / 0., "Can't divide by zero", std::runtime_error);
  }
  SUBCASE("testing /=") {
    Vector2 v1{1., 2.};
    Vector2 v2{2., 3.};
    Vector2 v3{4., 3.};
    Vector2 res1{1., 1.5};
    Vector2 res2{-4., -3.};
    v2 /= 2.;
    v3 /= -1.;
    CHECK(v2 == res1);
    CHECK(v3 == res2);
    CHECK_THROWS_WITH_AS(v1 / 0., "Can't divide by zero", std::runtime_error);
  }

  SUBCASE("testing magnitude") {
    Vector2 v1{1., 2.};
    Vector2 v2{2., 3.};
    Vector2 v3{4., 0.};
    CHECK(doctest::Approx(v2.magnitude()).epsilon(0.01) == sqrt(13.));
    CHECK(v3.magnitude() == 4.);
    CHECK(doctest::Approx(v1.magnitude()).epsilon(0.01) == sqrt(5.));
  }
  SUBCASE("testing +=") {
    Vector2 v1{1., 2.};
    Vector2 v2{2., 3.};
    Vector2 v3{4., 3.};
    Vector2 res1{3., 5.};
    Vector2 res2{6., 6.};
    Vector2 res3{5., 5.};
    v1 += v2;
    v2 += v3;
    CHECK(v1 == res1);
    CHECK(v2 == res2);
  }

  SUBCASE("Testing get angle") {
    Vector2 a{1, 1};
    Vector2 b{1, 0};
    Vector2 c{4, 10};
    Vector2 d{0, 0};
    Vector2 e{-1, 1};
    CHECK(doctest::Approx(get_angle(a, b)) == 0.785398);
    CHECK(doctest::Approx(get_angle(b, b)) == 0);
    CHECK(doctest::Approx(doctest::Approx(get_angle(b, Vector2{-1, 0})))
              .epsilon(0.001) == 3.1415);
    CHECK(doctest::Approx(get_angle(a, c)) == 0.40489);
    CHECK(doctest::Approx(get_angle(a, d)) == 0);
    CHECK(get_angle(a, e) == doctest::Approx(3.1415 / 2).epsilon(0.01));
  }
}