/* name : main.h
 * author : matheo thomas
 * date : 17-06-24
 */

#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>

#ifndef main_h 
#define main_h

/* Struct definitions */
typedef struct rect {
	int x;
	int y;
	int w;
	int h;
	int angle;
} rect;


/* Functions definitions */

/// Draws a rectangle at the defined coordinates, with the defined size and angle.
void drawRect(SDL_Renderer *renderer, rect r);

/// Creates a rectangle with a given width and height (the coordinates are the center of the window).
rect createRect(SDL_Window *window, int w, int h);


#endif
