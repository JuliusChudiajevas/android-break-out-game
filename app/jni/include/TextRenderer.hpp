#pragma once
#include <iostream>
#include <string>

#include "Game.hpp"
#include "SDL.h"
#include "SDLSmartPointers.hpp"
#include "SDL_ttf.h"

class TextRenderer {
 private:
  static SDLTTFFontPtr font;
  static int m_fontSize;
  static SDLSurfacePtr tempSurface;
  static SDLTexturePtr tempTexture;

 public:
  static void drawText(std::string text, int x, int y, int r, int g, int b, int fontSize) {
    if (m_fontSize != fontSize) {
      font.reset(TTF_OpenFont("Arial.ttf", fontSize));
      m_fontSize = fontSize;
    }
    if (font == nullptr) {
      SDL_Log("%s, %s\n", "No font found", TTF_GetError());
      return;
    }

    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;

    SDL_Rect rect;
    tempSurface.reset(TTF_RenderText_Solid(font.get(), text.c_str(), color));
    tempTexture.reset(SDL_CreateTextureFromSurface(Game::renderer, tempSurface.get()));
    rect.x = x;
    rect.y = y;
    rect.w = tempSurface->w;
    rect.h = tempSurface->h;
    SDL_RenderCopy(Game::renderer, tempTexture.get(), nullptr, &rect);
  }
};
