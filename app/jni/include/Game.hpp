#pragma once
#include <iostream>
#include <vector>

#include "Block.hpp"
#include "Entity.hpp"
#include "SDL.h"

inline void SDLLog(std::string text) {
  SDL_Log("%s\n", text.c_str());
}

inline void SDLLog(std::string text, float value) {
  SDL_Log("%s %s\n", text.c_str(), std::to_string(value).c_str());
}

inline void SDLLog(std::string text, int value) {
  SDL_Log("%s %s\n", text.c_str(), std::to_string(value).c_str());
}

inline void SDLLog(std::string text, long value) {
  SDL_Log("%s %s\n", text.c_str(), std::to_string(value).c_str());
}

inline void SDLLog(std::string text, unsigned long value) {
  SDL_Log("%s %s\n", text.c_str(), std::to_string(value).c_str());
}

class Game {
 private:
  static bool isRunning;
  SDL_Window* window;

 public:
  Game();
  ~Game();
  static float updateStartTime;
  static float updateEndTime;
  static float updateDeltaTime;
  static float elapsedTime;
  static float bestTime;

  static Scene* mainScene;
  static SDL_Renderer* renderer;

  void initWindow(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);

  static void initGameplay(Scene& scene, std::vector<std::vector<int>> map);

  void handleEvents();

  void update(Scene& scene);

  void render(Scene& scene);

  void cleanup();

  bool running();

  static void restart();
  static void gameOver();
  static void win();
  static void nextLevel();
  static void close() {
    isRunning = false;
  }
};