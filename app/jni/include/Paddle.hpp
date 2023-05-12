#pragma once
#include "Components.hpp"
#include "Entity.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Touch.hpp"
#include "Vec2d.hpp"
#include "entt.hpp"

class Paddle : public Entity {
 private:
  RectComponent& rect;
  SpriteComponent& sprite;
  bool isAttached;
  int lives;
  int score;

 public:
  Paddle(Scene& scene)
      : Entity(std::move(scene.createEntity())),
        rect(addComponent<RectComponent>(300, Screen::paddleTop, 100, 20)),
        sprite(addComponent<SpriteComponent>(128, 128, rect.width, rect.height)) {
    addTag<PaddleTagComponent>();
    addComponent<RectColorComponent>(100, 100, 255);
    addTag<CollisionTagComponent>();

    lives = 3;
    score = 0;
    isAttached = false;
  }

  RectComponent getRect() const {
    return rect;
  }

  void reduceLives() {
    lives--;
    if (lives <= 0) {
      Game::gameOver();
    }
  }

  void increaseScore() {
    score++;
  }

  int getScrore() const {
    return score;
  }

  int getLives() const {
    return lives;
  }

  void update(float deltaTime) {
    if (!Touch::isTouching) {
      isAttached = false;
      return;
    }
    if (isAttached) {
      rect.x = Touch::TouchPoint.x - (rect.width / 2);
      return;
    }
    if (Touch::TouchPoint.x >= rect.left() && Touch::TouchPoint.x <= rect.right()) {
      isAttached = true;
    }
  }

  void reset() {
    lives = 3;
    score = 0;
  }
};