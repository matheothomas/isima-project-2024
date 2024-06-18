#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *window =
      SDL_CreateWindow("fenetre", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);

  SDL_Renderer *renderer;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Texture *waves;
  waves = IMG_LoadTexture(renderer, "./img/waves.png");
  SDL_Texture *bonhomme;
  bonhomme = IMG_LoadTexture(
      renderer, "./img/126432-a3637f707cd1045fe7735b06dd5c39c7-2854974123.png");
  SDL_Texture *bg;
  bg = IMG_LoadTexture(renderer, "./img/nasa-xFO2Xt33xgI-unsplash.jpg");

  SDL_Rect window_dimensions;
  SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);

  SDL_Rect source_bg;
  SDL_QueryTexture(bg, NULL, NULL, &source_bg.w, &source_bg.h);
  SDL_Rect source_waves;
  SDL_QueryTexture(waves, NULL, NULL, &source_waves.w, &source_waves.h);
  SDL_Rect source_elephant;
  SDL_QueryTexture(bonhomme, NULL, NULL, &source_elephant.w,
                   &source_elephant.h);

  SDL_Rect destination_elephant;
  SDL_Rect state_elephant;

  SDL_Rect destination_waves;

  float zoom = 2;
  int offset_x = source_elephant.w / 12, offset_y = source_elephant.h / 8;

  state_elephant.x = 6 * offset_x;
  state_elephant.y = 1 * offset_y;
  state_elephant.w = offset_x;
  state_elephant.h = offset_y;

  destination_elephant.w = offset_x * zoom;
  destination_elephant.h = offset_y * zoom;

  destination_elephant.y =
      3 * (window_dimensions.h - destination_elephant.h) / 4;

  destination_waves.x = 0;
  destination_waves.y = window_dimensions.h - source_waves.h * 0.5;
  destination_waves.w = source_waves.w * 0.5;
  destination_waves.h = source_waves.h * 0.5;

  int speed_elephant = 4;
  int speed_waves = -4;

  for (int x = 0; x < window_dimensions.w - destination_elephant.w;
       x += speed_elephant) {
    destination_elephant.x = x;
    state_elephant.x += offset_x;
    if (state_elephant.x == 9 * offset_x) {
      state_elephant.x -= 3 * offset_x;
    }

    destination_waves.x += speed_waves;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bg, &source_bg, &window_dimensions);
    SDL_RenderCopy(renderer, waves, &source_waves, &destination_waves);
    SDL_RenderCopy(renderer, bonhomme, &state_elephant, &destination_elephant);
    SDL_RenderPresent(renderer);
    SDL_Delay(200);
  }

  SDL_DestroyTexture(waves);
  SDL_DestroyTexture(bonhomme);
  SDL_DestroyTexture(bg);

  IMG_Quit();
  return 0;
}