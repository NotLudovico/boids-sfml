#include "./headers/vectors.hpp"

#include <cmath>
#include <iostream>

Vector2::Vector2(float x, float y) : x_{x}, y_{y} {};
float Vector2::x() const { return x_; }
void Vector2::set_x(float new_x) { x_ = new_x; }
float Vector2::y() const { return y_; }
void Vector2::set_y(float new_y) { y_ = new_y; }
float Vector2::magnitude() const { return std::sqrt(x_ * x_ + y_ * y_); }
void Vector2::print() const { std::cout << x_ << ' ' << y_ << '\n'; }

bool operator==(Vector2 const& first, Vector2 const& second) {
  return first.x() == second.x() && first.y() == second.y();
}
bool operator!=(Vector2 const& first, Vector2 const& second) {
  return !(first == second);
}
Vector2 operator+(Vector2 const& first, Vector2 const& second) {
  return Vector2{first.x() + second.x(), first.y() + second.y()};
}
void operator+=(Vector2& first, Vector2 const& second) {
  first.set_x(first.x() + second.x());
  first.set_y(first.y() + second.y());
}
Vector2 operator-(Vector2 const& first, Vector2 const& second) {
  return Vector2{first.x() - second.x(), first.y() - second.y()};
}
void operator-=(Vector2& first, Vector2 const& second) {
  first.set_x(first.x() - second.x());
  first.set_y(first.y() - second.y());
}
Vector2 operator*(Vector2 const& first, float scalar) {
  return Vector2{first.x() * scalar, first.y() * scalar};
}
float operator*(Vector2 const& first, Vector2 const& second) {
  return first.x() * second.x() + first.y() * second.y();
}

void operator*=(Vector2& first, float scalar) {
  first.set_x(first.x() * scalar);
  first.set_y(first.y() * scalar);
}

float get_angle(Vector2 const& first, Vector2 const& second) {
  if (first.magnitude() == 0 || second.magnitude() == 0) return 0;

  return std::acos((first * second) / (first.magnitude() * second.magnitude()));
}

Vector2 operator/(Vector2 const& first, float scalar) {
  if (scalar == 0) throw std::runtime_error{"Can't divide by zero"};

  return Vector2{first.x() / scalar, first.y() / scalar};
}

void operator/=(Vector2& first, float scalar) {
  if (scalar == 0) throw std::runtime_error{"Can't divide by zero"};

  first.set_x(first.x() / scalar);
  first.set_y(first.y() / scalar);
}
