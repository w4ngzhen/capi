#include "SDL.h"
#include "SDL_image.h"

int main() {

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return -1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "Hello World",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800,
      600,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  SDL_SetWindowFullscreen(window, 0);

  if (!window) {
    return -1;
  }

  SDL_Renderer *win_renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {

      // 清空渲染器
      SDL_RenderClear(win_renderer);
      // 绘制矩形
      SDL_Rect rect{10, 10, 400, 300};
      SDL_SetRenderDrawColor(win_renderer, 0xFF, 0x00, 0x00, 0xFF);
      SDL_RenderFillRect(win_renderer, &rect);
      //呈现渲染器
      SDL_RenderPresent(win_renderer);

      if (e.type == SDL_MOUSEMOTION) {
        int rw = 0, rh = 0;
        SDL_GetRendererOutputSize(win_renderer, &rw, &rh);
        printf("rw = %4d, rh = %4d\n", rw, rh);
        printf("x = %4d, y = %4d\n", e.motion.x, e.motion.y);
        break;
      }

      if (e.type == SDL_QUIT) {
        quit = true;
        break;
      }
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}