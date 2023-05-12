#pragma once
#include "Components.hpp"
#include "TextRenderer.hpp"
#include "Touch.hpp"
#include "aabb.hpp"

class Button {
 private:
  std::function<void()> action;

 public:
  RectComponent rect;
  SDL_Color color;
  std::string text;
  float fontSize;

  Button(float x, float y, float width, float height, int r, int g, int b, std::string text,
         int fontSize, std::function<void()> callback) {
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    color.r = r;
    color.g = g;
    color.b = b;
    this->text = text;
    this->fontSize = fontSize;
    action = callback;
  }

  void executeAction() {
    action();
  }
};

class Menu {
 public:
  SDL_Color backgroundColor;
  std::vector<std::unique_ptr<Button>> buttons;

  Menu(int r, int g, int b, std::vector<std::unique_ptr<Button>> buttons)
      : buttons(std::move(buttons)) {
    backgroundColor.r = r;
    backgroundColor.g = g;
    backgroundColor.b = b;
  }

  void showMenu() {
    while (true) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_FINGERDOWN:
          case SDL_FINGERMOTION:
          case SDL_FINGERUP:
            Touch::update(event);
            break;
          default:
            break;
        }
      }
      if (Touch::isTouching) {
        for (auto &button : buttons) {
          auto touchPos = Touch::TouchPoint;
          auto buttonPos = button->rect;
          if (touchPos.x > buttonPos.x && touchPos.x < buttonPos.right() &&
              touchPos.y > buttonPos.top() && touchPos.y < buttonPos.bottom()) {
            button->executeAction();
            return;
          }
        }
      }
      clearScreen(backgroundColor.r, backgroundColor.g, backgroundColor.b);

      for (auto &button : buttons) {
        SDL_Rect temp;
        temp.x = button->rect.x;
        temp.y = button->rect.y;
        temp.w = button->rect.width;
        temp.h = button->rect.height;

        SDL_SetRenderDrawColor(Game::renderer, button->color.r, button->color.g, button->color.b,
                               button->color.a);

        SDL_RenderDrawRect(Game::renderer, &temp);
        SDL_RenderFillRect(Game::renderer, &temp);
        TextRenderer::drawText(button->text, button->rect.x, button->rect.y, 10, 10, 10,
                               button->fontSize);
      }
      SDL_RenderPresent(Game::renderer);
    }
  }
};