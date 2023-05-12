#include <iostream>

#include "Ball.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Game.hpp"
#include "Paddle.hpp"
#include "SDL.h"
#include "Scene.hpp"
#include "TextRenderer.hpp"
#include "TickTimer.hpp"
#include "aabb.hpp"
#include "entt.hpp"

extern bool aabb(RectComponent&, RectComponent&);
extern void clearScreen();

void spawnGround(Scene& scene, float x, float y, float width, float height) {
  Entity ground = scene.createEntity();
  ground.addTag<CollisionTagComponent>();
  ground.addTag<WallTagComponent>();
  auto groundRect = ground.addComponent<RectComponent>(x, y, width, height);
  ground.addComponent<RectColorComponent>(RectColorComponent{200, 200, 200});
}

int main(int argv, char** args) {
  std::unique_ptr<Game> game = std::make_unique<Game>();

  game->initWindow("game", 0, 0, 0, 0, true);

  TickTimer updateTimer(1000 / 250);
  TickTimer renderTimer(1000 / 144);

  Scene scene;
  game->initGameplay(scene, std::vector<std::vector<int>>{});
  // top
  spawnGround(scene, 0, 0, 1000, 10);
  // bottom
  spawnGround(scene, 0, 1500, 1000, 10);
  // left
  spawnGround(scene, 0, 0, 10, 1500);
  // right
  spawnGround(scene, 990, 0, 10, 1500);

  while (game->running()) {
    game->handleEvents();

    if (updateTimer.hasElapsed()) {
      game->update(scene);
    }

    if (renderTimer.hasElapsed()) {
      game->render(scene);
    }
  }

  game->cleanup();
  return 0;
}