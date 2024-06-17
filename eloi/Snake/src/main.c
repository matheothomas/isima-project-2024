#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "../include/main.h"


float min(float a, float b) {
	return (a > b) ? b : a;
}

float max(float a, float b) {
	return (a < b) ? b : a;
}


void end_sdl(char ok, char const* msg, SDL_Window* window, SDL_Renderer* renderer) {
	char msg_formated[255];                                            
	int l;                                                     

	if (!ok) {
		strncpy(msg_formated, msg, 250);                                         
		l = strlen(msg_formated);                                            
		strcpy(msg_formated + l, " : %s\n");                                     

		SDL_Log(msg_formated, SDL_GetError());                                   
	}                                                          

	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window != NULL)   {
		SDL_DestroyWindow(window);
		window= NULL;
	}

	SDL_Quit();                                                    

	if (!ok) { 
		exit(EXIT_FAILURE);                                                  
	}                                                          
}                                                        

void draw(SDL_Renderer* renderer, SDL_DisplayMode screen, Rect * rects, int nombre) {
	SDL_Rect rectangle;                                                

	SDL_SetRenderDrawColor(renderer,                                       
						0, 0, 0, 255);
	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = screen.w;
	rectangle.h = screen.h;

	SDL_RenderFillRect(renderer, &rectangle);                        

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	for (int i = 0; i < nombre - 1; i++) {
		drawRectangle(renderer, rects[i]);
	} 

}

void drawRectangle(SDL_Renderer * renderer, Rect rectangle) {

	float angle = atan2f((rectangle.p1 -> y - rectangle.p2 -> y), (rectangle.p2 -> x - rectangle.p1 -> x));
	int dirX = rectangle.p2 -> x - rectangle.p1 -> x;
	int dirY = rectangle.p2 -> y - rectangle.p1 -> y;
	float norme = sqrtf((float) dirX*dirX + dirY*dirY);
	float rayon = rectangle.w / 2 * max(0.5, min(2, rectangle.length*sqrtf(rectangle.length)/norme));
	int x_offset = (int) (rayon * cosf(angle + M_PI/2));
	int y_offset = - (int) (rayon * sinf(angle + M_PI/2));
	int x1 = rectangle.p1 -> x + x_offset;
	int y1 = rectangle.p1 -> y + y_offset;
	int x4 = rectangle.p1 -> x - x_offset;
	int y4 = rectangle.p1 -> y - y_offset;
	int x2 = rectangle.p2 -> x + x_offset;
	int y2 = rectangle.p2 -> y + y_offset;
	int x3 = rectangle.p2 -> x - x_offset;
	int y3 = rectangle.p2 -> y - y_offset;
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
	SDL_RenderDrawLine(renderer, x3, y3, x4, y4);
	SDL_RenderDrawLine(renderer, x4, y4, x1, y1);
}

void update_speed(Point * point, float speedX, float speedY) {
	if (!point -> fixed) {
		point -> speedX += speedX;
		point -> speedY += speedY;
	}
}

void update_speed_points(Rect * rects, int nombre) {
	
	float dt = 0.01;
	float k = 2;
	float gravity = 2;
	int taille = 30;
	float damping = 0.05;

	for (int i = 0; i < nombre - 1; i++) {

		int dirX = rects[i].p2 -> x - rects[i].p1 -> x;
		int dirY = rects[i].p2 -> y - rects[i].p1 -> y;
		float norme = sqrtf((float) dirX*dirX + dirY*dirY);
		float dirX_normalized = dirX / norme;
		float dirY_normalized = dirY / norme;

		float elastic_force_X = -k * (norme - rects[i].length) * dirX_normalized;
		float elastic_force_Y = -k * (norme - rects[i].length) * dirY_normalized;
		
		// Au cas ou on rajoute des forces supplémentaires
		float speedX = (elastic_force_X);
		float speedY = (elastic_force_Y + gravity);

		update_speed(rects[i].p1, -elastic_force_X - damping * rects[i].p1 -> speedX, -elastic_force_Y - damping * rects[i].p1 -> speedY + gravity);
		update_speed(rects[i].p2, elastic_force_X, elastic_force_Y + gravity);

	}

}

void update_pos_points(Point * points, Rect * rects, int nombre) {

	update_speed_points(rects, nombre);

	for (int i = 0; i < nombre; i++) {
		points[i].x += points[i].speedX * 0.1;
		points[i].y += points[i].speedY * 0.1;
	}

}

int main(int argc, char** argv) {

	srand(time(NULL)); // Init random generator

	(void)argc;
	(void)argv;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event;

	SDL_DisplayMode screen;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) end_sdl(0, "ERROR SDL INIT", window, renderer);

	SDL_GetCurrentDisplayMode(0, &screen);
	printf("Résolution écran\n\tw : %d\n\th : %d\n",
		screen.w, screen.h);

	/* Création de la fenêtre */
	window = SDL_CreateWindow("Premier dessin",
						   SDL_WINDOWPOS_CENTERED,
						   SDL_WINDOWPOS_CENTERED, screen.w * 0.90,
						   screen.h * 0.90,
	SDL_WINDOW_OPENGL);
	if (window == NULL) end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	/* Création du renderer */
	renderer = SDL_CreateRenderer(window, -1,
							   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) end_sdl(0, "ERROR RENDERER CREATION", window, renderer);


	// Création de la corde élastique

	int height;
	int width;

	SDL_GetWindowSize(window, &width, &height);

	int nombre = 180;
	int size = 10;
	int centerX = width / 2;
	int centerY = height / 2;
	Point * points = (Point *) malloc(sizeof(Point) * nombre);
	Rect * rects = (Rect *) malloc(sizeof(Rect) * (nombre - 1));
	for (int i = 0; i < nombre; i++) {
		points[i].x = centerX + 600 * cosf(2*M_PI/nombre*i) * sinf(2*M_PI/nombre * i);
		points[i].y = centerY + 500 * sinf(2*M_PI/nombre*i);
		points[i].speedX = 0.;
		points[i].speedY = 0.;
		points[i].accX = 0.;
		points[i].accY = 0.;
		points[i].fixed = !(i % 10);
	}

	for (int j = 0; j < nombre - 1; j++) {
		rects[j].p1 = &points[j];
		rects[j].p2 = &points[j+1];
		rects[j].w = 10;
		rects[j].length = 10;
	}

	int mouseX = rand() % width;
	int mouseY = rand() % height;
	int held = 0;

	while (1) {


		update_pos_points(points, rects, nombre);

		draw(renderer, screen, rects, nombre);
		SDL_RenderPresent(renderer);
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_WINDOWEVENT:
					switch (event.window.event) {

						case SDL_WINDOWEVENT_CLOSE:
							end_sdl(1, "Normal ending", window, renderer);
							return EXIT_SUCCESS;
							break;
					
						case SDL_WINDOWEVENT_SIZE_CHANGED:
						break;
					}
				break;
				
				case SDL_QUIT:
					end_sdl(1, "Normal ending", window, renderer);
					return EXIT_SUCCESS;
					break;

				case SDL_MOUSEBUTTONDOWN:
					held = !held;
					break;
			}
		}

		if (held) {
			SDL_GetGlobalMouseState(&mouseX, &mouseY);
			points[nombre-1].x = (float) mouseX - 0.1*screen.w/2;
			points[nombre-1].y = (float) mouseY - 0.1*screen.h/2;
			points[nombre-1].speedX = 0;
			points[nombre-1].speedY = 0;
		}
		SDL_Delay(10);


	}
	end_sdl(1, "Normal ending", window, renderer);
	return EXIT_SUCCESS;
}
