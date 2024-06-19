/* name : graphics.h
 * author : matheo thomas
 * date : 18-06-24
 */

#include <SDL2/SDL.h>

#ifndef graphics_h 
#define graphics_h



/* Functions definitions */

/// ends the SDL elements and handles errors
void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer);

/// loads a texture from an image
SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer );

/// displays the textures on the renderer
void texturing(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer);



#endif

