#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../include/main.h"

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

	SDL_RenderDrawLine(renderer, rectangle.p1 -> x, rectangle.p1 -> y, rectangle.p2 -> x, rectangle.p2 -> y);
	float angle = atan2f((rectangle.p1 -> y - rectangle.p2 -> y), (rectangle.p2 -> x - rectangle.p1 -> x));
	int rayon = rectangle.w / 2;
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
	float k = 1.5;
	float gravity = 3;
	int taille = 100;
	float damping = 0.015;

	for (int i = 0; i < nombre - 1; i++) {

		int dirX = rects[i].p2 -> x - rects[i].p1 -> x;
		int dirY = rects[i].p2 -> y - rects[i].p1 -> y;
		float norme = sqrtf((float) dirX*dirX + dirY*dirY);
		float dirX_normalized = dirX / norme;
		float dirY_normalized = dirY / norme;

		float elastic_force_X = -k * (norme - taille) * dirX_normalized;
		float elastic_force_Y = -k * (norme - taille) * dirY_normalized;
		
		// Au cas ou on rajoute des forces supplémentaires
		float speedX = (elastic_force_X);
		float speedY = (elastic_force_Y + gravity);

		update_speed(rects[i].p1, -elastic_force_X - damping * rects[i].p1 -> speedX, -elastic_force_Y - damping * rects[i].p1 -> speedY + gravity);
		update_speed(rects[i].p2, elastic_force_X, elastic_force_Y + gravity);

		printf("AAAA %f\n", rects[1].p2->speedY * 0.1);

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
						   SDL_WINDOWPOS_CENTERED, screen.w * 0.66,
						   screen.h * 0.66,
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

	int nombre = 10;
	int size = 100;
	int centerX = width / 2;
	int centerY = height / 2;
	Point * points = (Point *) malloc(sizeof(Point) * size);
	Rect * rects = (Rect *) malloc(sizeof(Rect) * (size - 1));
	for (int i = 0; i < nombre; i++) {
		points[i].x = centerX + i * size;
		points[i].y = 50 + i * size * 0;
		points[i].speedX = 0.;
		points[i].speedY = 0.;
		points[i].accX = 0.;
		points[i].accY = 0.;
		points[i].fixed = 0;
	}
	points[0].fixed = 1;

	for (int j = 0; j < nombre - 1; j++) {
		rects[j].p1 = &points[j];
		rects[j].p2 = &points[j+1];
		rects[j].w = 10;
	}

	time_t now = time(0);

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
			}
		}
		SDL_Delay(1);


	}
	end_sdl(1, "Normal ending", window, renderer);
	return EXIT_SUCCESS;
}
