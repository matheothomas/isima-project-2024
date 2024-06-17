

#ifndef main_h
#define main_h
#endif

//structs 

typedef struct Point {
	int x;
	int y;
	int speed;
	int acc;
} Point;

typedef struct Rect {
	Point * p1;
	Point * p2;
	int w;
	int r;
	int g;
	int b;
} Rect;

//functions
void end_sdl(char ok, char const* msg, SDL_Window* window, SDL_Renderer* renderer);
void draw(SDL_Renderer * renderer);
void drawRectangle(SDL_Renderer* renderer, Rect rectangle);

