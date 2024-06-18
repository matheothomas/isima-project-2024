#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <time.h>

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  int h;
  int w;

  SDL_Window *window =
      SDL_CreateWindow("fenetre", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);

  SDL_Renderer *renderer;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Rect rect;
  rect.x = 600 / 4;
  rect.y = 600 / 4;
  rect.h = 42;
  rect.w = 42;

  int x1;
  int x2;
  int y1;
  int y2;

  int r = 255;
  int g = 250;
  int b = 0;
  int multicolor = SDL_FALSE;

  srand(time(0));

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, r, g, b, 255);

  SDL_bool program_on = SDL_TRUE;
  SDL_Event event;
  while (program_on) {
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        program_on = SDL_FALSE;
        break;
      case SDL_MOUSEBUTTONDOWN:
        r = rand() % 255;
        g = rand() % 255;
        b = rand() % 255;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_SPACE:
          multicolor = 1;
          while (multicolor) {
            if (SDL_PollEvent(&event)) {
              switch (event.type) {
              case SDL_QUIT:
                multicolor = SDL_FALSE;
                program_on = SDL_FALSE;
                break;
              case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                  multicolor = SDL_FALSE;
                case SDLK_c:
                  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                  SDL_RenderClear(renderer);
                  break;
                default:
                  break;
                }
              default:
                break;
              }
              SDL_Delay(1);
            }
            SDL_GetWindowSize(window, &w, &h);
            int x;
            int y;
            x = rand() % w;
            y = rand() % h;
            r = rand() % 255;
            g = rand() % 255;
            b = rand() % 255;
            x1 = rect.x + rect.w / 2;
            y1 = rect.y + rect.h / 2;
            x2 = x;
            y2 = y;
            rect.x = x - rect.w / 2;
            rect.y = y - rect.h / 2;

            SDL_SetRenderDrawColor(renderer, r, g, b, 100);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            SDL_RenderFillRect(renderer, &rect);
            SDL_RenderPresent(renderer);
          }
        case SDLK_r:
          r = 255;
          g = 0;
          b = 0;
          break;
        case SDLK_g:
          g = 255;
          r = 0;
          b = 0;
          break;
        case SDLK_b:
          b = 255;
          g = 0;
          r = 0;
          break;
        case SDLK_y:
          b = 255;
          g = 255;
          r = 0;
          break;
        case SDLK_p:
          b = 255;
          g = 0;
          r = 255;
          break;
        case SDLK_c:
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          SDL_RenderClear(renderer);
          break;
        default:
          break;
        }
      default:
        break;
      }
      SDL_Delay(1);
    }
    SDL_GetWindowSize(window, &w, &h);
    int x;
    int y;
    x = rand() % w;
    y = rand() % h;
    x1 = rect.x + rect.w / 2;
    y1 = rect.y + rect.h / 2;
    x2 = x;
    y2 = y;
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;

    SDL_SetRenderDrawColor(renderer, r, g, b, 100);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
