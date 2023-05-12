#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "SDL_image.h"
#include "TextureManager.hpp"

struct TagComponent {
  std::string tag;
};

class Vec2dComponent {
 public:
  float x;
  float y;

  Vec2dComponent() {
    x = 0;
    y = 0;
  }

  Vec2dComponent(float p_x, float p_y) {
    x = p_x;
    y = p_y;
  }

  Vec2dComponent operator-(const Vec2dComponent& b) const {
    return Vec2dComponent(x - b.x, y - b.y);
  }

  Vec2dComponent operator+(const Vec2dComponent& b) const {
    return Vec2dComponent(x + b.x, y + b.y);
  }

  Vec2dComponent operator*(float a) const {
    return Vec2dComponent(x * a, y * a);
  }
  Vec2dComponent operator/(float a) const {
    return Vec2dComponent(x / a, y / a);
  }

  float lenght() const {
    return sqrt(x * x + y * y);
  }

  Vec2dComponent normalized() const {
    return Vec2dComponent(*this / lenght());
  }
};

struct RectComponent {
  float x, y, width, height;
  RectComponent() : x(0), y(0), width(10), height(10) {
  }
  RectComponent(float x, float y, float width, float height)
      : x(x), y(y), width(width), height(height) {
  }
  float left() {
    return x;
  }
  float right() {
    return x + width;
  }
  float top() {
    return y;
  }
  float bottom() {
    return y + height;
  }
};

struct PlayerTagComponent {};
struct PaddleTagComponent {};
struct BallTagComponent {};

struct KeyboardComponent {
  std::map<SDL_Keycode, bool> keys;
  KeyboardComponent(std::vector<SDL_Keycode>&& assignedKeys) {
    for (auto key : assignedKeys) {
      keys.emplace(key, false);
    }
  }
};

struct SpriteComponent {
  SDL_Texture* texture = nullptr;
  SDL_Rect srcRect, dstRect;

  SpriteComponent() {
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 32;
    srcRect.h = 32;

    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = 32;
    dstRect.h = 32;
  }

  SpriteComponent(float width, float height) {
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = width;
    srcRect.h = height;

    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = width;
    dstRect.h = height;
  }

  SpriteComponent(float srcWidth, float srcHeight, float dstWidth, float dstHeight) {
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = srcWidth;
    srcRect.h = srcHeight;

    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = dstWidth;
    dstRect.h = dstHeight;
  }
  SpriteComponent(float srcWidth, float srcHeight, float dstWidth, float dstHeight,
                  const char* texturePath) {
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = srcWidth;
    srcRect.h = srcHeight;

    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = dstWidth;
    dstRect.h = dstHeight;

    texture = TextureManager::loadTexture(texturePath);
  }
};

struct CollisionTagComponent {};

struct ProjectileTagComponent {};

struct WallTagComponent {};

struct RectColorComponent {
  SDL_Color color;

  RectColorComponent() {
    color.r = 255;
    color.g = 0;
    color.b = 0;
    color.a = 255;
  }

  RectColorComponent(int r, int g, int b) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;
  }

  RectColorComponent(int r, int g, int b, int a) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
  }
};

struct GravityComponent {
  float gravity = 0.00002;
};

struct AccelerationComponent {
  float xAcceleration = 0.0;
  float yAcceleration = 0.0;
};

struct VelocityComponent {
  float xVelocity = 0.0;
  float yVelocity = 0.0;
  float xMaxVelocity = 1;
  float yMaxVelocity = 1;
  VelocityComponent() {
  }
  VelocityComponent(float xMaxVelocity, float yMaxVelocity)
      : xMaxVelocity(xMaxVelocity), yMaxVelocity(yMaxVelocity) {
  }
  VelocityComponent(float xVelocity, float yVelocity, float xMaxVelocity, float yMaxVelocity)
      : xVelocity(xVelocity),
        yVelocity(yVelocity),
        xMaxVelocity(xMaxVelocity),
        yMaxVelocity(yMaxVelocity) {
  }
};

struct LivesComponent {
  int lives = 0;
  LivesComponent(int lives) : lives(lives) {
  }
};

struct IsInAirComponent {
  bool isInAir = true;
};

struct ButtonComponent {
  bool isHovered = false;
  std::string text;
};
