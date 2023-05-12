#pragma once

#include <cmath>
class Vec2d {
 public:
  float x;
  float y;

  Vec2d() {
    x = 0;
    y = 0;
  }

  Vec2d(float p_x, float p_y) {
    x = p_x;
    y = p_y;
  }

  Vec2d operator-(const Vec2d &b) const {
    return Vec2d(x - b.x, y - b.y);
  }

  Vec2d operator+(const Vec2d &b) const {
    return Vec2d(x + b.x, y + b.y);
  }

  Vec2d operator*(float a) const {
    return Vec2d(x * a, y * a);
  }
  Vec2d operator/(float a) const {
    return Vec2d(x / a, y / a);
  }

  float lenght() const {
    return sqrt(x * x + y * y);
  }

  Vec2d normalized() const {
    return Vec2d(*this / lenght());
  }
};