#pragma once
#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"

struct SDL_Deleter {
  void operator()(SDL_Surface* ptr) {
    if (ptr) SDL_FreeSurface(ptr);
  }
  void operator()(SDL_Texture* ptr) {
    if (ptr) SDL_DestroyTexture(ptr);
  }
  void operator()(SDL_Renderer* ptr) {
    if (ptr) SDL_DestroyRenderer(ptr);
  }
  void operator()(SDL_Window* ptr) {
    if (ptr) SDL_DestroyWindow(ptr);
  }
  void operator()(SDL_RWops* ptr) {
    if (ptr) SDL_RWclose(ptr);
  }
  void operator()(TTF_Font* ptr) {
    if (ptr) TTF_CloseFont(ptr);
  }
};

// Unique Pointers
using SDLSurfacePtr = std::unique_ptr<SDL_Surface, SDL_Deleter>;
using SDLTexturePtr = std::unique_ptr<SDL_Texture, SDL_Deleter>;
using SDLRendererPtr = std::unique_ptr<SDL_Renderer, SDL_Deleter>;
using SDLWindowPtr = std::unique_ptr<SDL_Window, SDL_Deleter>;
using SDLRWopsPtr = std::unique_ptr<SDL_RWops, SDL_Deleter>;
using SDLTTFFontPtr = std::unique_ptr<TTF_Font, SDL_Deleter>;