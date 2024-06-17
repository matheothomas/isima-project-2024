

#ifndef main_h
#define main_h
#endif

//structs 

typedef struct Point {
	float x;
	float y;
	float speedX;
	float speedY;
	float accX;
	float accY;
	int fixed;
} Point;

typedef struct Rect {
	Point * p1;
	Point * p2;
	int w;
	float length;
	int r;
	int g;
	int b;
} Rect;

//functions
void end_sdl(char ok, char const* msg, SDL_Window* window, SDL_Renderer* renderer);
void draw(SDL_Renderer * renderer, SDL_DisplayMode screen, Rect * rects, int nombre);
void drawRectangle(SDL_Renderer* renderer, Rect rectangle);

