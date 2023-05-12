#pragma once

#include "Components.hpp"
#include "Entity.hpp"
#include "Game.hpp"

class Block : public Entity {
 private:
  RectComponent& rect;
  LivesComponent& lives;
  RectColorComponent& color;

 public:
  Block(Scene& scene)
      : Entity(std::move(scene.createEntity())),
        rect(addComponent<RectComponent>(0, 0, 120, 40)),
        lives(addComponent<LivesComponent>(1)),
        color(addComponent<RectColorComponent>(255, 255, 255)) {
    addTag<CollisionTagComponent>();
  }

  void setX(float x) {
    rect.x = x;
  }

  void setY(float y) {
    rect.y = y;
  }

  float getWidth() {
    return rect.width;
  }

  float getHeight() {
    return rect.height;
  }

  void setWidth(float newWidth) {
    rect.width = newWidth;
  }

  void setHeight(float newHeight) {
    rect.height = newHeight;
  }

  void setLives(int newLives) {
    lives.lives = newLives;
  }

  int getLives() const {
    return lives.lives;
  }
  RectComponent getRect() const {
    return rect;
  }

  entt::entity getEntityHandle() {
    return entityHandle;
  }

  void updateOnHit() {
    lives.lives--;
  }
};