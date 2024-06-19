/* name : graphics.h
 * author : matheo thomas
 * date : 18-06-24
 */

#include <SDL2/SDL.h>

#ifndef graphics_h 
#define graphics_h



/* Functions definitions */

void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer);

SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer );

void texturing(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer);



#endif