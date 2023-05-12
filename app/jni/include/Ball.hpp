
#pragma once
#include "Block.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Paddle.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Vec2d.hpp"
#include "aabb.hpp"
#include "entt.hpp"

class Ball : public Entity {
 private:
  float startingPosX = 500;
  float startingPosY = 700;
  float startingVelocityY = 190.0;
  RectComponent& rect;
  SpriteComponent& sprite;
  VelocityComponent& velocity;

  void updateCollisionX(std::unique_ptr<Paddle>& paddle,
                        std::vector<std::unique_ptr<Block>>& blocks) {
    auto view2 = scene->registry.view<CollisionTagComponent, RectComponent>();

    for (auto [entity2, rect2] : view2.each()) {
      // dont collide to yourself
      if (entityHandle == entity2) {
        continue;
      }

      // if no collision
      if (!aabb(rect, rect2)) {
        continue;
      }

      float overlapX = 0;

      float xOverlapFromLeft = rect.right() - rect2.left();
      float xOverlapFromRight = rect2.right() - rect.left();

      if (std::abs(xOverlapFromLeft) < std::abs(xOverlapFromRight)) {
        overlapX = -xOverlapFromLeft;
      } else {
        overlapX = xOverlapFromRight;
      }
      velocity.xVelocity *= -1;
      rect.x += overlapX;
      rect.x = std::round(rect.x);
    }
  }

  void adjustAfterCollisionY(RectComponent otherRect) {
    float overlapY = 0;

    float yOverlapFromTop = rect.bottom() - otherRect.top();
    float yOverlapFromBottom = otherRect.bottom() - rect.top();

    if (std::abs(yOverlapFromTop) < std::abs(yOverlapFromBottom)) {
      overlapY = -yOverlapFromTop;
    } else {
      overlapY = yOverlapFromBottom;
    }

    velocity.yVelocity *= -1;

    rect.y += overlapY;
    rect.y = std::round(rect.y);
    // SDLLog("X:", rect.x);
    // SDLLog("Y:", rect.y);
  }

  void updateCollisionY(std::unique_ptr<Paddle>& paddle,
                        std::vector<std::unique_ptr<Block>>& blocks) {
    // paddle collision logic
    auto paddleRect = paddle->getRect();
    if (aabb(rect, paddleRect)) {
      float paddleMidPoint = paddleRect.left() + (paddleRect.width / 2.0);
      float ballMidPoint = rect.left() + (rect.width / 2);
      float diff = ballMidPoint - paddleMidPoint;
      velocity.xVelocity += diff;
      velocity.yVelocity *= 1.1;
      if (velocity.xVelocity > 500) {
        velocity.xVelocity = 500;
      }
      if (velocity.yVelocity > 500) {
        velocity.yVelocity = 500;
      }
      adjustAfterCollisionY(paddleRect);
      SDLLog("PADDLE HIT, diff: ", diff);
      return;
    }

    for (auto itr = blocks.begin(); itr != blocks.end(); ++itr) {
      auto& block = *itr;

      if (block == nullptr) {
        SDLLog("NULLPTR BLOCK");
        continue;
      }
      auto blockRect = block->getRect();
      if (aabb(rect, blockRect)) {
        adjustAfterCollisionY(blockRect);
        block->updateOnHit();
        if (block->getLives() == 0) {
          scene->registry.destroy(block->getEntityHandle());
          block.reset(nullptr);
          blocks.erase(itr);
          paddle->increaseScore();
//          SDLLog("BLOCK size: ", blocks.size());
        }
        return;
      }
    }

    if (blocks.empty()) {
      Game::win();
    }

    // adjust ball movement
    auto collideable =
        scene->registry.view<WallTagComponent, CollisionTagComponent, RectComponent>();
    for (auto [collideableHandle, collideableRect] : collideable.each()) {
      // dont collide to yourself
      if (entityHandle == collideableHandle) {
        continue;
      }

      // if no collision
      if (!aabb(rect, collideableRect)) {
        continue;
      }

      adjustAfterCollisionY(collideableRect);
      // SDLLog("COLLISION Y");
      // SDLLog("X:", rect.x);
      // SDLLog("Y:", rect.y);
    }
  }

  void checkForDeath(std::unique_ptr<Paddle>& paddle) {
    if (rect.y > Screen::paddleTop) {
      reset();
      paddle->reduceLives();
      SDLLog("RESET ball");
    }
  }

  void updateMovement(float deltaTime) {
  }

 public:
  Ball(Scene& scene)
      : Entity(std::move(scene.createEntity())),
        rect(addComponent<RectComponent>(startingPosX, startingPosY, 32, 32)),
        sprite(addComponent<SpriteComponent>(128, 128, rect.width, rect.height, "ball.png")),
        velocity(addComponent<VelocityComponent>(000.0, startingVelocityY, 1000.0, 1000.0)) {
    addComponent<RectColorComponent>();
    addTag<BallTagComponent>();
  }

  void update(float deltaTime, std::unique_ptr<Paddle>& paddle,
              std::vector<std::unique_ptr<Block>>& blocks) {
    rect.x += velocity.xVelocity * deltaTime;
    updateCollisionX(paddle, blocks);
    rect.y += velocity.yVelocity * deltaTime;
    updateCollisionY(paddle, blocks);
    checkForDeath(paddle);
  }

  void reset() {
    rect.x = startingPosX;
    rect.y = startingPosY;
    velocity.yVelocity = startingVelocityY;
    velocity.xVelocity = 0.0;
  }
};