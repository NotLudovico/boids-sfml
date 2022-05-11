#ifndef VECTORS_HPP
#define VECTORS_HPP
#include <cmath>
#include <vector>

class Vector2 {
 private:
  float x_{};
  float y_{};

 public:
  Vector2(float x, float y);
  float x() const;
  void set_x(float new_x);
  float y() const;
  void set_y(float new_y);
  float magnitude() const;
  void print() const;
};

bool operator==(Vector2 const& first, Vector2 const& second);
bool operator!=(Vector2 const& first, Vector2 const& second);
Vector2 operator+(Vector2 const& first, Vector2 const& second);
void operator+=(Vector2& first, Vector2 const& second);
Vector2 operator-(Vector2 const& first, Vector2 const& second);
void operator-=(Vector2& first, Vector2 const& second);
Vector2 operator*(Vector2 const& first, float scalar);
float operator*(Vector2 const& first, Vector2 const& second);
void operator*=(Vector2& first, float scalar);
Vector2 operator/(Vector2 const& first, float scalar);
void operator/=(Vector2& first, float scalar);
float get_angle(Vector2 const& first, Vector2 const& second);

#endif