#pragma once

#include "Game.hpp"
#include "SDL.h"
#include "Screen.hpp"

class Touch {
 public:
  static SDL_Point TouchPoint;
  static int TouchId;
  static bool isTouching;

 public:
  Touch() {
  }
  static void update(SDL_Event event) {
    // no multi touct
    if (event.tfinger.fingerId != 0) {
      return;
    }
    switch (event.type) {
      case SDL_FINGERDOWN:
        isTouching = true;

        // SDLLog("touch update");
        // SDLLog("touch id: ", event.tfinger.fingerId);

        TouchPoint.x = event.tfinger.x * Screen::Width;
        TouchPoint.y = event.tfinger.y * Screen::Height;
        // SDLLog("x: ", TouchPoint.x);
        // SDLLog("y: ", TouchPoint.y);
        break;
      case SDL_FINGERMOTION:
        // SDLLog("finger motion update");

        TouchPoint.x = event.tfinger.x * Screen::Width;
        TouchPoint.y = event.tfinger.y * Screen::Height;
        // SDLLog("x: ", TouchPoint.x);
        // SDLLog("y: ", TouchPoint.y);
        break;
      case SDL_FINGERUP:
        isTouching = false;
        // SDLLog("finger UP update");

        break;
    }
  }
};