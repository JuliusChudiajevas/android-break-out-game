#include "Game.hpp"

#include <math.h>

#include <iostream>
#include <map>
#include <vector>

#include "Ball.hpp"
#include "Block.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Menu.hpp"
#include "Paddle.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Scene.hpp"
#include "Screen.hpp"
#include "TextRenderer.hpp"
#include "Touch.hpp"
#include "aabb.hpp"

int Screen::Width = 0;
int Screen::Height = 0;
float Screen::paddleTop = 1000;
SDL_Point Touch::TouchPoint;
int Touch::TouchId;
bool Touch::isTouching = false;

Game::Game() {
}

Game::~Game() {
}

SDL_Renderer* Game::renderer = nullptr;
bool Game::isRunning = false;
float Game::updateDeltaTime = 1;
float Game::updateEndTime = 1;
float Game::updateStartTime = 1;
float Game::elapsedTime = 0;
float Game::bestTime = 0;
Scene* Game::mainScene = nullptr;

SDLTTFFontPtr TextRenderer::font;
SDLTexturePtr TextRenderer::tempTexture;
SDLSurfacePtr TextRenderer::tempSurface;
int TextRenderer::m_fontSize = 0;

void Game::initWindow(const char* title, int xPos, int yPos, int width = 0, int height = 0,
                      bool fullscreen = true) {
  std::cout << "initializing" << std::endl;
  int flags = 0;

  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    isRunning = false;
    return;
  }

  if (TTF_Init()) {
    std::cout << "TTF not initialized " << TTF_GetError() << std::endl;
    return;
  }

  SDL_DisplayMode mode;
  if (width == 0 || height == 0) {
    if (SDL_GetCurrentDisplayMode(0, &mode)) {
      SDLLog("MODE FAILED!");
    }
    SDLLog("Window ASDF ", 5);
    SDLLog("Window width ", mode.w);
    SDLLog("Window height ", mode.h);
    // window = SDL_CreateWindow(title, xPos, yPos, mode.w, mode.h, flags);
    window = SDL_CreateWindow(title, xPos, yPos, mode.w, mode.h, flags);
  } else
    window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);

  if (window) {
    std::cout << "window created" << std::endl;
    SDLLog("Window created! k");
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    std::cout << "renderer created" << std::endl;
  }

  float scaleFactor = mode.w / 1000.0;
  SDLLog("scale: ", scaleFactor);

  SDL_RenderSetLogicalSize(renderer, mode.w, mode.h);
  SDL_RenderSetScale(renderer, scaleFactor, scaleFactor);

  Screen::Width = mode.w / scaleFactor;
  Screen::Height = mode.h / scaleFactor;

  isRunning = true;
}

std::unique_ptr<Paddle> paddle;
std::unique_ptr<Ball> ball;

std::vector<std::vector<int>> map1 = {{1, 1, 1, 1}, {0, 1, 1, 0}};
std::vector<std::vector<int>> map2 = {{1, 1, 1, 1}, {1, 1, 1, 1}};
std::vector<std::vector<int>> currentMap;
std::vector<std::unique_ptr<Block>> blocks;

void loadBlocksFromMap(Scene& scene, std::vector<std::vector<int>> map) {
  if (map.size() == 0) {
    map = map1;
  }
  int maxHorizontalBlocks = map[0].size();
  int maxVerticalBlocks = map.size();
  float startingX = 260;
  float startingY = 200;
  float blockWidth = 120;
  float blockHeight = 40;
  float gap = 10;
  for (int i = 0; i < maxVerticalBlocks; i++) {
    for (int j = 0; j < maxHorizontalBlocks; j++) {
      if (map[i][j] > 0) {
        auto tempBlock = std::make_unique<Block>(scene);

        tempBlock->setWidth(blockWidth);
        tempBlock->setHeight(blockHeight);

        tempBlock->setY(startingY + gap * i + blockHeight * i);
        tempBlock->setX(startingX + gap * j + blockWidth * j);
        blocks.push_back(std::move(tempBlock));
      }
    }
  }
  currentMap = map;
}

std::unique_ptr<Menu> gameOverMenu;
std::unique_ptr<Menu> winMenu;

void Game::initGameplay(Scene& scene, std::vector<std::vector<int>> map) {
  mainScene = &scene;
  paddle = std::make_unique<Paddle>(scene);
  ball = std::make_unique<Ball>(scene);
  loadBlocksFromMap(scene, map);

  std::unique_ptr<Button> exitButton =
      std::make_unique<Button>(600, 700, 250, 150, 150, 50, 50, "EXIT", 40, Game::close);
  std::unique_ptr<Button> restartButton1 =
      std::make_unique<Button>(200, 700, 250, 150, 200, 200, 200, "RESTART", 40, Game::restart);

  std::vector<std::unique_ptr<Button>> gameOverMenuButtons;
  gameOverMenuButtons.push_back(std::move(exitButton));
  gameOverMenuButtons.push_back(std::move(restartButton1));

  gameOverMenu = std::make_unique<Menu>(0, 0, 0, std::move(gameOverMenuButtons));

  std::unique_ptr<Button> nextLevelButton = std::make_unique<Button>(
      600, 700, 250, 150, 200, 200, 200, "NEXT LEVEL", 40, Game::nextLevel);
  std::unique_ptr<Button> restartButton2 =
      std::make_unique<Button>(200, 700, 250, 150, 200, 200, 200, "RESTART", 40, Game::restart);

  std::vector<std::unique_ptr<Button>> nextLevelMenuButtons;
  nextLevelMenuButtons.push_back(std::move(nextLevelButton));
  nextLevelMenuButtons.push_back(std::move(restartButton2));

  winMenu = std::make_unique<Menu>(0, 0, 0, std::move(nextLevelMenuButtons));
}

void Game::gameOver() {
  gameOverMenu->showMenu();
}

void Game::win() {
  winMenu->showMenu();
}

void Game::restart() {
  for (auto& block : blocks) {
    Game::mainScene->registry.destroy(block->getEntityHandle());
  }
  blocks.clear();
  ball->reset();
  paddle->reset();
  loadBlocksFromMap(*Game::mainScene, currentMap);
}

void Game::nextLevel() {
  for (auto& block : blocks) {
    Game::mainScene->registry.destroy(block->getEntityHandle());
  }
  blocks.clear();
  ball->reset();
  paddle->reset();
  loadBlocksFromMap(*Game::mainScene, map2);
}

void Game::handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_FINGERDOWN:
      case SDL_FINGERMOTION:
      case SDL_FINGERUP:
        Touch::update(event);
        break;
      case SDL_QUIT:
        isRunning = false;
        break;
      default:
        break;
    }
  }
}

void Game::update(Scene& scene) {
  updateEndTime = SDL_GetTicks64() / 1000.0;
  updateDeltaTime = updateEndTime - updateStartTime;
  if (updateDeltaTime > 0.015) updateDeltaTime = 0.015;

  paddle->update(updateDeltaTime);
  ball->update(updateDeltaTime, paddle, blocks);
  updateStartTime = updateEndTime;
}

void Game::render(Scene& scene) {
  clearScreen(10, 10, 10);
  auto view = scene.registry.view<RectComponent, RectColorComponent>();
  for (auto [entity, rect, rectColor] : view.each()) {
    SDL_Rect temp;
    temp.x = rect.x;
    temp.y = rect.y;
    temp.w = rect.width;
    temp.h = rect.height;
    SDL_SetRenderDrawColor(renderer, rectColor.color.r, rectColor.color.g, rectColor.color.b,
                           rectColor.color.a);

    SDL_RenderDrawRect(renderer, &temp);
    SDL_RenderFillRect(renderer, &temp);
  }
  TextRenderer::drawText("LIVES: " + std::to_string(paddle->getLives()), 750, 1200, 200, 200, 200,
                         50);
  TextRenderer::drawText("SCORE: " + std::to_string(paddle->getScrore()), 50, 1200, 200, 200, 200,
                         50);
  SDL_RenderPresent(renderer);
}

void Game::cleanup() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

bool Game::running() {
  return isRunning;
}