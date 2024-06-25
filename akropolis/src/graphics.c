/* name : graphics.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <time.h>

#include "algos.h"
#include "graphics.h"
#include "init.h"


void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer) {              // renderer to close
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
    TTF_Init();
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
    SDL_Rect *window_dimensions, *board_player, *board_bot;
	colours_t *colours;
	SDL_Texture **type_texture;
	SDL_Texture *background;
	TTF_Font * font;
	float offset_x;
	float offset_y;

	type_texture_t *types;

    SDL_DisplayMode screen;

    // SDL INITIALISATION
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		end_sdl(0, "ERROR SDL INIT", window, renderer);

	SDL_GetCurrentDisplayMode(0, &screen);

    // Window creation
	window = SDL_CreateWindow("Premier dessin", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 
                            screen.w * 0.9,
						    screen.h * 0.9, SDL_WINDOW_OPENGL);

	if (window == NULL)
		end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	// Font opening
	font=TTF_OpenFont("res/Unique.ttf", 72 );

	// Rects creation
	window_dimensions=crea_rect(0, 0, screen.w * 0.9, screen.h * 0.9);

	board_player=crea_rect(0, 0, window_dimensions->w/3, window_dimensions->h);
	board_bot=crea_rect(window_dimensions->w/3, 0, window_dimensions->w/3, window_dimensions->h);

	offset_y=(float)board_player->h/61;
	offset_x=2*0.866*offset_y;
	//offset_x=(float)board_player->h/40;
	//offset_y=offset_x/(0.866*2);

    // Renderer creation
	renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

	colours=init_colours();

	types=init_type_texture( window, renderer);

	type_texture=malloc(12*sizeof(SDL_Texture*));

	type_texture[0]=types->empty;
	type_texture[1]=types->blue;
	type_texture[2]=types->red;
	type_texture[3]=types->yellow;
	type_texture[4]=types->purple;
	type_texture[5]=types->green;
	type_texture[6]=types->grey;
	type_texture[7]=types->blue_place;
	type_texture[8]=types->red_place;
	type_texture[9]=types->yellow_place;
	type_texture[10]=types->purple_place;
	type_texture[11]=types->green_place;


    // Textures creation
	background= load_texture_from_image("res/background.jpeg", window, renderer);
	background=NULL;


	printf("x : %f y : %f \n", offset_x, offset_y);

    graphics_t *graphics = malloc(sizeof(graphics_t));

    graphics->window=window;
    graphics->renderer=renderer;
    graphics->window_dimensions=window_dimensions;
    graphics->background=background;
    graphics->colours=colours;
	graphics->type_texture=type_texture;
    graphics->font=font;
	graphics->offset_x=offset_x;
	graphics->offset_y=offset_y;

	//free(types);

    return graphics;
}

colours_t *init_colours(){

	colours_t *colours=malloc(sizeof(colours_t));

	SDL_Color yellow = {255,250,0,255}; 
	SDL_Color black = {0,0,0,255};
	SDL_Color dark_red = {25,20,20,255};
	SDL_Color white = {255,255,255,255};

	colours->yellow=yellow;
	colours->black=black;
	colours->dark_red=dark_red;
	colours->white=white;

	return colours;
}

type_texture_t *init_type_texture( SDL_Window *window, SDL_Renderer *renderer){

	type_texture_t *type_texture=malloc(sizeof(type_texture_t));

	SDL_Texture *empty, *blue, *red, *yellow, *green, *purple, *grey, *blue_place, *red_place, *yellow_place, *green_place, *purple_place;
	
	empty= load_texture_from_image("res/empty.png", window, renderer);
	blue= load_texture_from_image("res/blue.png", window, renderer);
    red= load_texture_from_image("res/red.png", window, renderer);
    yellow= load_texture_from_image("res/yellow.png", window, renderer);
    purple= load_texture_from_image("res/purple.png", window, renderer);
    green= load_texture_from_image("res/green.png", window, renderer);
    grey= load_texture_from_image("res/gray.png", window, renderer);
	blue_place= load_texture_from_image("res/blue_star.png", window, renderer);
    red_place= load_texture_from_image("res/red_star.png", window, renderer);
    yellow_place= load_texture_from_image("res/yellow_star.png", window, renderer);
    purple_place= load_texture_from_image("res/purple_star.png", window, renderer);
    green_place= load_texture_from_image("res/green_star.png", window, renderer);

	type_texture->empty=empty;
	type_texture->blue=blue;
    type_texture->red=red;
    type_texture->yellow=yellow;
    type_texture->purple=purple;
    type_texture->green=green;
    type_texture->grey=grey;
	type_texture->blue_place=blue_place;
    type_texture->red_place=red_place;
    type_texture->yellow_place=yellow_place;
    type_texture->purple_place=purple_place;
    type_texture->green_place=green_place;

	return type_texture;
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

SDL_Texture* create_texture_for_text(char  *  text, TTF_Font * font, SDL_Renderer *renderer, SDL_Color colour){       
	SDL_Surface * my_text = TTF_RenderText_Blended(font, text, colour);
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

SDL_Rect* crea_rect_in_rect(SDL_Rect *button, float i, float j, float k, float l){
	SDL_Rect *dir=crea_rect(button->x+((float)i*(button->w)), button->y+((float)j*(button->h)), k*(button->w), l*(button->h));
	return dir;
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

int is_in_hexa (SDL_Rect* dest, int x, int y, int offset_x, int offset_y){
	int is_in = 0;
	int x_bis = x - dest->x;
	int y_bis = y - dest->y;
	if(x_bis * offset_y + y_bis * offset_x > offset_x * offset_y){
		if(x_bis * offset_y - y_bis * offset_x < offset_x * offset_y){
			if(x_bis < dest->w){
				if((x_bis - offset_x) * offset_y + (y_bis - 3 * offset_y) * offset_x < offset_x * offset_y){
					if(x_bis * offset_y - (y_bis - 4 * offset_y) * offset_x > offset_x * offset_y){
						if(x_bis > 0){
							is_in = 1;
						}
					}
				}
			}
		}
	}
	return is_in;
}

void display_cell(SDL_Texture *texture, graphics_t *graphics, int id, int altitude, int decal) {
	SDL_Rect source = {0}, destination = {0}, destination_alt = {0};

	char *string_altitude= malloc(sizeof(char));
	sprintf(string_altitude, "%d",altitude);
	SDL_Texture *texture_altitude=create_texture_for_text(string_altitude, graphics->font, graphics->renderer, graphics->colours->white);

	destination.w = 2 * graphics->offset_x;
	destination.h = 4 * graphics->offset_y;

	destination_alt.w = 0.5 * graphics->offset_x;
	destination_alt.h = 1.5 * graphics->offset_y;

	int i=id%39;

	if(i>=0 && i<=19){
		destination.x=2*i * graphics->offset_x +decal;
		destination.y=6*((int)id/39) * graphics->offset_y;

		destination_alt.x=(2*i+0.75) * graphics->offset_x +decal;
		destination_alt.y=(6*((int)id/39)+2) * graphics->offset_y;
	}
	else if (i>=20 && i<=38){
		destination.x=(((2*id)%39)) * graphics->offset_x +decal;
		destination.y=(6*((int)id/39)+3) * graphics->offset_y;

		destination_alt.x=(((2*id)%39)+0.75) * graphics->offset_x +decal;
		destination_alt.y=(6*((int)id/39)+5) * graphics->offset_y;
	}

	//printf("x : %d y : %d id : %d\n", destination.x,destination.y, id);

	SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(graphics->renderer, texture, &source, &destination);

	SDL_QueryTexture(texture_altitude, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(graphics->renderer, texture_altitude, &source, &destination_alt);

	SDL_DestroyTexture(texture_altitude);
	free(string_altitude);
}

int get_cell_id_from_mouse_position(graphics_t *graphics, int x, int y, int decal){
	int id=-1;
	int i=0;
	SDL_Rect destination = {0};
	destination.w = 2 * graphics->offset_x;
	destination.h = 4 * graphics->offset_y;
	for(int k=0;k<390;k+=39){
		for(i=k;i<k+20;i++){
			destination.x=2*i * graphics->offset_x +decal;
			destination.y=6*((int)id/39) * graphics->offset_y;
			if(is_in_hexa(&destination, x, y, graphics->offset_x, graphics->offset_y)){
				id=i;
				k=390;
				i=390;
			}
		}
		for(i=k+38;i>k+19;i--){
			destination.x=(((2*id)%39)) * graphics->offset_x +decal;
			destination.y=(6*((int)id/39)+3) * graphics->offset_y;
			if(is_in_hexa(&destination, x, y, graphics->offset_x, graphics->offset_y)){
				id=i;
				k=390;
				i=390;
			}
		}
	}
	return id;
}



void display_board(graphics_t *g, board_t * board, int decal) {
	//SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	//SDL_RenderClear(g->renderer);

	// display all cells
	for(int i=0;i<390;i++){
		//if(game->player->cell_tab[i]->level->cell_type){
			display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
		//}
	}

	//SDL_RenderPresent(g->renderer);
}

void display_game(graphics_t* g, game_t *game){
	
	SDL_Rect source = {0};

	// clear renderer
	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	SDL_RenderClear(g->renderer);

	// background
	//SDL_QueryTexture(g->background, NULL, NULL, &source.w, &source.h);
	//SDL_RenderCopy(g->renderer, g->background, &source, g->window_dimensions);

	// panel

	// board
	display_board(g, game->player,0);
	SDL_RenderDrawRect(g->renderer, g->board_bot);
	display_board(g, game->bot,g->window_dimensions->w/3);

	// text

	// text box

	// shows
	SDL_RenderPresent(g->renderer);
}
