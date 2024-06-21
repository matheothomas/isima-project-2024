/* name : graphics.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#include "algos.h"
#include "graphics.h"
#include "init.h"

void end_sdl(char ok,                               // normal end : ok = 0 ; abnormal ok = 1
			 char const *msg,                       // message to print
			 SDL_Window *window,                    // window to close
			 SDL_Renderer *renderer) {              // renderer to close
	char msg_formated[255];                                            
	int l;                                                     

	if (!ok) {										// printing what is wrong
		strncpy(msg_formated, msg, 250);                                         
		l = strlen(msg_formated);                                            
		strcpy(msg_formated + l, " : %s\n");                                     

		SDL_Log(msg_formated, SDL_GetError());                                   
	}                                                          

	if (renderer != NULL) {							// renderer's destruction if necessary
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window != NULL)   {                         // windows destruction if necessary
		SDL_DestroyWindow(window);
		window= NULL;
	}

	SDL_Quit();                                                    

	if (!ok) {
		exit(EXIT_FAILURE);                                                  
	}            
}

graphics_t *init_sdl() {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_DisplayMode screen;
	SDL_Texture *board, *white, *black, *red, *config_1, *config_2, *commands;
	TTF_Font * font;

	// SDL INITIALISATION
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		end_sdl(0, "ERROR SDL INIT", window, renderer);

	SDL_GetCurrentDisplayMode(0, &screen);

	// Window creation
	window = SDL_CreateWindow("Premier dessin", SDL_WINDOWPOS_CENTERED,
						   SDL_WINDOWPOS_CENTERED, screen.h * 0.9 * 1.5,
						   screen.h * 0.9, SDL_WINDOW_OPENGL);

	if (window == NULL)
		end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	// Renderer creation
	renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

	// Textures creation
	board = load_texture_from_image("res/board.png", window, renderer);
	white = load_texture_from_image("res/white.png", window, renderer);
	black = load_texture_from_image("res/black.png", window, renderer);
	red = load_texture_from_image("res/red.png", window, renderer);
	config_1 = load_texture_from_image("res/config_1.png", window, renderer);
	config_2 = load_texture_from_image("res/config_2.png", window, renderer);
	commands = load_texture_from_image("res/commands.png", window, renderer);

	// Font opening
	font=TTF_OpenFont("res/Unique.ttf", 72 );

	// Commands_panel creation
	commands_panel_t *c= init_commands_panel(screen.h * 0.9 * 1.5, screen.h * 0.9);
	
	graphics_t *graphics = malloc(sizeof(graphics_t));
	graphics->window = window;
	graphics->renderer = renderer;
	graphics->board = board;
	graphics->white = white;
	graphics->black = black;
	graphics->red = red;
	graphics->config_1 = config_1;
	graphics->config_2 = config_2;
	graphics->commands = commands;
	graphics->font = font;
	graphics->commands_panel=c;

	return graphics;
}

commands_panel_t *init_commands_panel(int w, int h){
	commands_panel_t* commands_panel=malloc(sizeof(commands_panel_t));
	
	SDL_Rect *button = crea_rect(13*(w)/18, 6*(h)/11, 2*(w)/9, 2*(w)/9);

	commands_panel->tab_dir[0] = crea_rect_in_rect(button, (float)1/3, (float)2/9);
	commands_panel->tab_dir[1] = crea_rect_in_rect(button, (float)2/3, (float)2/9);
	commands_panel->tab_dir[2] = crea_rect_in_rect(button, (float)5/6, (float)1/2);
	commands_panel->tab_dir[3] = crea_rect_in_rect(button, (float)2/3, (float)7/9);
	commands_panel->tab_dir[4] = crea_rect_in_rect(button, (float)1/3, (float)7/9);
	commands_panel->tab_dir[5] = crea_rect_in_rect(button, (float)1/6, (float)1/2);
	
	commands_panel->button=button;

	return commands_panel;
}

SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer ){
	SDL_Surface *my_image = NULL;
	SDL_Texture* my_texture = NULL;

	my_image = IMG_Load(file_image_name);
	if (my_image == NULL) end_sdl(0, "Chargement de l'image impossible", window, renderer);

	my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
	SDL_FreeSurface(my_image);
	if (my_texture == NULL) end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);

	return my_texture;
}

SDL_Texture* create_texture_for_text(char  *  text, TTF_Font * font, SDL_Renderer *renderer ){
	SDL_Color yellow = {255,250,0,0};        
	SDL_Surface * my_text = TTF_RenderText_Blended(font, text, yellow);
	SDL_Texture * my_texture = SDL_CreateTextureFromSurface(renderer, my_text);
	SDL_FreeSurface(my_text);
	return my_texture;
}

SDL_Rect* crea_rect(int x, int y, int width, int height){
	SDL_Rect* rect=(SDL_Rect*)malloc(sizeof(SDL_Rect));
	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
	return rect;
}

SDL_Rect* crea_rect_in_rect(SDL_Rect *button, float i, float j){
	SDL_Rect *dir=crea_rect(button->x+((float)i*(button->w))-(float)(button->w)/9, button->y+((float)j*(button->h))-(float)(button->h)/9, 2*(button->w)/9, 2*(button->h)/9);
	return dir;
}

void texturing(SDL_Texture* texture, SDL_Window* window, SDL_Renderer* renderer) {
	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect source = {0}, window_dimensions = {0}, destination = {0};

	SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
	SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);

	float zoom = 0.9;
	destination.w = window_dimensions.h * zoom;						// the destination is a source's zoom
	destination.h = window_dimensions.h * zoom;
	destination.x = (window_dimensions.h - destination.w) / 2;
	destination.y = (window_dimensions.h - destination.h) / 2;


	// SDL_RenderCopyEx(renderer, my_texture, &source, &destination, j*0.1, &point, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, texture, &source, &destination);
}

void display_cell(SDL_Texture *texture, SDL_Window *window, SDL_Renderer *renderer, int id) {
	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect source = {0}, window_dimensions = {0}, destination = {0};

	int i, j;
	float k;

	SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
	SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);

	float zoom = 10;
	destination.w = window_dimensions.h / (zoom + 1.6);
	destination.h = window_dimensions.h / (zoom + 3.2);
	
	if(id < 5) {
		i = 1;
		j = id;
		k = 2;
	} else if (id < 11) {
		i = 2;
		j = id - 5;
		k = 1.5;
	} else if (id < 18) {
		i = 3;
		j = id - 11;
		k = 1;
	} else if (id < 26) {
		i = 4;
		j = id - 18;
		k = 0.5;
	} else if (id < 35) {
		i = 5;
		j = id - 26;
		k = 0;
	} else if (id < 43) {
		i = 6;
		j = id - 35;
		k = 0.5;
	} else if (id < 50) {
		i = 7;
		j = id - 43;
		k = 1;
	} else if (id < 56) {
		i = 8;
		j = id - 50;
		k = 1.5;
	} else if (id < 61) {
		i = 9;
		j = id - 56;
		k = 2;
	}

	destination.x = destination.w * (j+k+1.3);
	destination.y = destination.h * (i+1.2);

	SDL_RenderCopy(renderer, texture, &source, &destination);
	
}

int get_cell_id_from_mouse_position(graphics_t *g, int x, int y) {
	SDL_Rect window_dimensions;

	int i, w, h, y2;
	float k;
	int id = -1; // TO BE CHANGED

	SDL_GetWindowSize(g->window, &window_dimensions.w, &window_dimensions.h);
	
	float zoom = 10;
	w = window_dimensions.h / (zoom + 1.6);
	h = window_dimensions.h / (zoom + 3.2);

	i = 1;
	k = 2;
	y2 = h * (i+1.2);
	if(y > y2 && y < y2 + h) {
		return (float)x / w + 0 - k - 1.3;
	}

	i = 2;
	k = 1.5;
	y2 = h * (i+1.2);
	if(y > y2 && y < y2 + h) {
		return (float)x / w + 5 - k - 1.3;
	}

	i = 3;
	k = 1;
	y2 = h * (i+1.2);
	if(y > y2 && y < y2 + h) {
		return (float)x / w + 11 - k - 1.3;
	}

	i = 4;
	k = 0.5;
	y2 = h * (i+1.2);
	if(y > y2 && y < y2 + h) {
		return (float)x / w + 18 - k - 1.3;
	}

	i = 5;
	k = 0;
	y2 = h * (i+1.2);
	if(y > y2 && y < y2 + h) {
		return (float)x / w + 26 - k - 1.3;
	}

	i = 6;
	k = 0.5;
	y2 = h * (i+1.2);
	if(y > y2 && y < y2 + h) {
		return (float)x / w + 35 - k - 1.3;
	}

	i = 7;
	k = 1;
	y2 = h * (i+1.2);
	if(y > y2 && y < y2 + h) {
		return (float)x / w + 43 - k - 1.3;
	}
	
	i = 8;
	k = 1.5;
	y2 = h * (i+1.2);
	if(y > y2 && y < y2 + h) {
		return (float)x / w + 50 - k - 1.3;
	}
	
	i = 9;
	k = 2;
	y2 = h * (i+1.2);
	if(y > y2 && y < y2 + h) {
		return (float)x / w + 56 - k - 1.3;
	}

	return id;
}

void display_board(graphics_t *g, cell_t **cell_tab) {
	SDL_RenderClear(g->renderer);
	texturing(g->board, g->window, g->renderer);

	for(int i = 0; i < 61; i++) {
		if(cell_tab[i]->selection != UNSELECT) {
			display_cell(g->red, g->window, g->renderer, i);
		} else if(cell_tab[i]->state == WHITE) {
			display_cell(g->white, g->window, g->renderer, i);
		} else if (cell_tab[i]->state == BLACK) {
			display_cell(g->black, g->window, g->renderer, i);
		}
	}

	// SDL_RenderPresent(g->renderer);
}

int is_in (SDL_Rect* button,int x,int y){
	int is_in=0;
	if(x>button->x && x<button->x+button->w){
		if(y>button->y && y<button->y+button->h){
			is_in=1;
		}
	}
	return is_in;
}

void home_menu(graphics_t* g,SDL_Rect* text_box,SDL_Rect* button_1,SDL_Rect* button_2,SDL_Texture * text,int r1,int r2){
	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	SDL_RenderClear(g->renderer);

	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(g->renderer, text_box);

	SDL_SetRenderDrawColor(g->renderer, r1, 200, 200, 255);
	SDL_RenderFillRect(g->renderer, button_1);

	SDL_SetRenderDrawColor(g->renderer, r2, 200, 200, 255);
	SDL_RenderFillRect(g->renderer, button_2);


	SDL_Rect source = {0};

	SDL_QueryTexture(g->config_1, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->config_1, &source, button_1);

	SDL_QueryTexture(g->config_1, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->config_2, &source, button_2);

	SDL_QueryTexture(text, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, text, NULL, text_box);


	SDL_RenderPresent(g->renderer);
}

void display_game(graphics_t* g,SDL_Rect* text_box,SDL_Rect* confirm,SDL_Texture * text, int r, cell_t **cell_tab, int direction_state){
	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	SDL_RenderClear(g->renderer);

	display_board(g, cell_tab);

	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(g->renderer, text_box);

	SDL_SetRenderDrawColor(g->renderer, r, 200, 200, 255);
	SDL_RenderFillRect(g->renderer, confirm);

	SDL_RenderDrawRect(g->renderer, g->commands_panel->button);
	SDL_Rect source = {0};
	SDL_QueryTexture(g->commands, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->commands, &source, g->commands_panel->button);


	for(int i=0;i<6;i++){
		if(i==direction_state){
			SDL_QueryTexture(g->red, NULL, NULL, &source.w, &source.h);
			SDL_RenderCopy(g->renderer, g->red, &source, g->commands_panel->tab_dir[i]);
		}
	}

	SDL_QueryTexture(text, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, text, NULL, text_box);

	SDL_RenderPresent(g->renderer);
}
