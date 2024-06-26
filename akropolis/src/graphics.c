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
    SDL_Rect *window_dimensions, *displayed_board, *mini_board, *panel;

	SDL_Rect *bot;
	SDL_Rect *score_bot;
	SDL_Rect *player;
	SDL_Rect *score_player;
	SDL_Rect *left_arrow;
	SDL_Rect *right_arrow;
	SDL_Rect *first_tile;
	SDL_Rect *second_tile;
	SDL_Rect *deck;
	SDL_Rect *tiles_in_deck;
	SDL_Rect *bot_rocks;
	SDL_Rect *player_rocks;
	SDL_Rect *n_bot_rocks;
	SDL_Rect *n_player_rocks;
	SDL_Rect *rocks_price_1;
	SDL_Rect *rocks_price_2;
	SDL_Rect *price_1;
	SDL_Rect *price_2;

	SDL_Texture *bot_text;
	SDL_Texture *player_text;

	SDL_Texture *rocks;

	SDL_Texture *left_arrow_text;
	SDL_Texture *right_arrow_text;

	SDL_Texture *text_price_1;
	SDL_Texture *text_price_2;

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
                            screen.h * 1.5,
						    screen.h * 0.9, SDL_WINDOW_OPENGL);

	if (window == NULL)
		end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	// Font opening
	font=TTF_OpenFont("res/Unique.ttf", 72 );

	// Rects creation
	window_dimensions=crea_rect(0, 0, screen.h * 1.5, screen.h * 0.9);

	offset_y=(float)window_dimensions->h/61;
	offset_x=2*0.866*offset_y;

	displayed_board=crea_rect(0, 0, 40*offset_x, window_dimensions->h);

	int ratio=window_dimensions->w - 47 * offset_x;
	mini_board=crea_rect(40*offset_x, 0, ratio, 61 * ratio/69.28);
	panel=crea_rect(40*offset_x, 0, window_dimensions->w - 40 * offset_x, window_dimensions->h);

	bot=crea_rect(window_dimensions->w - 7 * offset_x, 0, 0.6 * 7 * offset_x, 61 * ratio/(4*69.28));
	score_bot=crea_rect(window_dimensions->w - 7 * offset_x + 0.6 * 7 * offset_x, 0, 0.4 * 7 * offset_x, 61 * ratio/(4*69.28));
	player=crea_rect(window_dimensions->w - 7 * offset_x, 2 * 61 * ratio/(4*69.28), 0.6 * 7 * offset_x, 61 * ratio/(4*69.28));
	score_player=crea_rect(window_dimensions->w - 7 * offset_x + 0.6 * 7 * offset_x, 2 * 61 * ratio/(4*69.28), 0.4 * 7 * offset_x, 61 * ratio/(4*69.28));

	bot_rocks=crea_rect(window_dimensions->w - 7 * offset_x + 0.1 * 7 * offset_x, 61 * ratio/(4*69.28), 0.4 * 7 * offset_x, 61 * ratio/(4*69.28));
	n_bot_rocks=crea_rect(window_dimensions->w - 7 * offset_x + 0.5 * 7 * offset_x, 61 * ratio/(4*69.28), 0.5 * 7 * offset_x, 61 * ratio/(4*69.28));

	player_rocks=crea_rect(window_dimensions->w - 7 * offset_x + 0.1 * 7 * offset_x, 3 * 61 * ratio/(4*69.28), 0.4 * 7 * offset_x, 61 * ratio/(4*69.28));
	n_player_rocks=crea_rect(window_dimensions->w - 7 * offset_x + 0.5 * 7 * offset_x, 3 * 61 * ratio/(4*69.28), 0.5 * 7 * offset_x, 61 * ratio/(4*69.28));

	left_arrow=crea_rect(40*offset_x + (window_dimensions->w - 40 * offset_x)/5, 61 * ratio/69.28 + 4 * offset_y, 4 * (window_dimensions->w - 40 * offset_x)/15, 6 * offset_y);
	right_arrow=crea_rect(40*offset_x + 8 * (window_dimensions->w - 40 * offset_x)/15 , 61 * ratio/69.28 + 4 * offset_y, 4 * (window_dimensions->w - 40 * offset_x)/15, 6 * offset_y);

	first_tile=crea_rect( 40*offset_x + (window_dimensions->w - 53.43 * offset_x)/2, 61 * ratio/69.28 + 14 * offset_y, 5.71 * offset_x, 10 * offset_y);
	second_tile=crea_rect(40*offset_x + (window_dimensions->w - 53.43 * offset_x)/2 + 7.71 * offset_x, 61 * ratio/69.28 + 14 * offset_y, 5.71 * offset_x, 10 * offset_y);


	rocks_price_1=crea_rect( 40*offset_x + (window_dimensions->w - 53.43 * offset_x)/2, 61 * ratio/69.28 + 24 * offset_y, 0.4 * 7 * offset_x, 61 * ratio/(4*69.28));
	price_1=crea_rect(40*offset_x + (window_dimensions->w - 53.43 * offset_x)/2 + 0.4 * 7 * offset_x, 61 * ratio/69.28 + 24 * offset_y, 0.5 * 7 * offset_x, 61 * ratio/(4*69.28));

	rocks_price_2=crea_rect( 40*offset_x + (window_dimensions->w - 53.43 * offset_x)/2 + 7.71 * offset_x, 61 * ratio/69.28 + 24 * offset_y, 0.4 * 7 * offset_x, 61 * ratio/(4*69.28));
	price_2=crea_rect(40*offset_x + (window_dimensions->w - 53.43 * offset_x)/2 + 7.71 * offset_x + 0.4 * 7 * offset_x, 61 * ratio/69.28 + 24 * offset_y, 0.5 * 7 * offset_x, 61 * ratio/(4*69.28));


	deck=crea_rect(40*offset_x + (window_dimensions->w - 40 * offset_x)/2 - 2 * offset_x, window_dimensions->h - 7 * offset_y, 4 * offset_x, 7 * offset_y);
	tiles_in_deck=crea_rect(40*offset_x + (window_dimensions->w - 40 * offset_x)/2 - offset_x, window_dimensions->h - 4 * offset_y, 4 * offset_x, 4 * offset_y);

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
	type_texture[8]=types->yellow_place;
	type_texture[9]=types->red_place;
	type_texture[10]=types->purple_place;
	type_texture[11]=types->green_place;


    // Textures creation
	background= load_texture_from_image("res/background.jpeg", window, renderer);
	background=NULL;

	bot_text=create_texture_for_text(" Bot: ", font, renderer, colours->white);
	player_text=create_texture_for_text(" You: ", font, renderer, colours->white);

	left_arrow_text=load_texture_from_image("res/arrow_left.png", window, renderer);
	right_arrow_text=load_texture_from_image("res/arrow_right.png", window, renderer);

	rocks=load_texture_from_image("res/rocks.png", window, renderer);

	text_price_1=create_texture_for_text(" 0 ", font, renderer, colours->white);
	text_price_2=create_texture_for_text(" 1 ", font, renderer, colours->white);


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
	graphics->displayed_board=displayed_board;
	graphics->mini_board=mini_board;
	graphics->panel=panel;

	graphics->bot_rect=bot;
	graphics->score_bot=score_bot;
	graphics->player_rect=player;
	graphics->score_player=score_player;

	graphics->left_arrow=left_arrow;
	graphics->right_arrow=right_arrow;

	graphics->rocks=rocks;
	graphics->bot_rocks=bot_rocks;
	graphics->n_bot_rocks=n_bot_rocks;
	graphics->player_rocks=player_rocks;
	graphics->n_player_rocks=n_player_rocks;

	graphics->first_tile=first_tile;
	graphics->second_tile=second_tile;

	graphics->rocks_price_1=rocks_price_1;
	graphics->rocks_price_2=rocks_price_2;
	graphics->price_1=price_1;
	graphics->price_2=price_2;

	graphics->text_price_1=text_price_1;
	graphics->text_price_2=text_price_2;

	graphics->deck=deck;
	graphics->tiles_in_deck=tiles_in_deck;

	graphics->bot_text=bot_text;
	graphics->player_text=player_text;

	graphics->left_arrow_text=left_arrow_text;
	graphics->right_arrow_text=right_arrow_text;

	free(types);

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

int is_in_hexa (SDL_Rect dest, int x, int y, int offset_x, int offset_y){
	int is_in = 0;
	int x_bis = x - dest.x;
	int y_bis = y - dest.y;
	if(x_bis * offset_y + y_bis * offset_x > offset_x * offset_y){
		if(x_bis * offset_y - y_bis * offset_x < offset_x * offset_y){
			if(x_bis < dest.w){
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

	char *string_altitude= malloc(2*sizeof(char));
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
			destination.x=2*i%39 * graphics->offset_x +decal;
			destination.y=6*((int)i/39) * graphics->offset_y;
			if(is_in_hexa(destination, x, y, graphics->offset_x, graphics->offset_y)){
				id=i;
				k=390;
				i=390;
			}
		}
		for(i=k+20;i<k+39;i++){
			destination.x=(((2*i)%39)) * graphics->offset_x +decal;
			destination.y=(6*((int)i/39)+3) * graphics->offset_y;
			if(is_in_hexa(destination, x, y, graphics->offset_x, graphics->offset_y)){
				id=i;
				k=390;
				i=390;
			}
		}
	}
	return id;
}

void display_board(graphics_t *g, game_t *game, board_t * board, int decal) {
///*
	if(game->selected_card==1){
		if(game->card_1->orientation%2){
			for(int i=0;i<390;i++){
				if( board->cell_tab[i]->neighbour[5] && board->cell_tab[i]->neighbour[2]){
					if(!(board->cell_tab[i]->selection==MOUSE) && !(board->cell_tab[i]->neighbour[5]->selection==MOUSE)){
						display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
					}
					else if (board->cell_tab[i]->selection==MOUSE && board->cell_tab[i]->neighbour[3] && board->cell_tab[i]->neighbour[0] && board->cell_tab[i]->neighbour[1]){
						if(board->cell_tab[i]->neighbour[0]->neighbour[0] && board->cell_tab[i]->neighbour[0]->neighbour[5] && board->cell_tab[i]->neighbour[1]->neighbour[2]){
							display_mouse_cells(game->card_1, board->cell_tab[i], g);
						}
					}
				}
				else{
					display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
				}
			}
		}
		else{
			for(int i=0;i<390;i++){
				if(!(board->cell_tab[i]->selection==MOUSE)){
					display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
				}
				else if (board->cell_tab[i]->selection==MOUSE){
					display_mouse_cells(game->card_1, board->cell_tab[i], g);
				}
				else{
					display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
				}
			}
		}
	}
	else if(game->selected_card==2){
		if(game->card_2->orientation%2){
			for(int i=0;i<390;i++){
				if( board->cell_tab[i]->neighbour[5] && board->cell_tab[i]->neighbour[2]){
					if(!(board->cell_tab[i]->selection==MOUSE) && !(board->cell_tab[i]->neighbour[5]->selection==MOUSE)){
						display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
					}
					else if (board->cell_tab[i]->selection==MOUSE && board->cell_tab[i]->neighbour[3] && board->cell_tab[i]->neighbour[0] && board->cell_tab[i]->neighbour[1]){
						if(board->cell_tab[i]->neighbour[0]->neighbour[0] && board->cell_tab[i]->neighbour[0]->neighbour[5] && board->cell_tab[i]->neighbour[1]->neighbour[2]){
							display_mouse_cells(game->card_2, board->cell_tab[i], g);
						}
					}
				}
				else{
					display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
				}
			}
		}
		else{
			for(int i=0;i<390;i++){
				if(!(board->cell_tab[i]->selection==MOUSE)){
					display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
				}
				else if (board->cell_tab[i]->selection==MOUSE){
					display_mouse_cells(game->card_2, board->cell_tab[i], g);
				}
				else{
					display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
				}
			}
		}
	}
	else {
		for(int i=0;i<390;i++){
			if(!(board->cell_tab[i]->selection==MOUSE)){
				display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
			}
		}
	}
//*/

/*
	// display all cells
	for(int i=0;i<390;i++){
		if(!(board->cell_tab[i]->selection==MOUSE)){
			display_cell(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, decal);
		}
	}
	// "drag" for drag and drop
	for(int i=0;i<390;i++){
		if (board->cell_tab[i]->selection==MOUSE) {
			if(game->selected_card==1){
				display_mouse_cells(game->card_1, board->cell_tab[i], g);
			}
			if(game->selected_card==2){
				display_mouse_cells(game->card_2, board->cell_tab[i], g);
			}
		}
	}
*/
}


void display_cell_bis(SDL_Texture *texture, graphics_t *graphics, int id, int altitude, int offset_x, int offset_y, int decal_x, int decal_y) {
	SDL_Rect source = {0}, destination = {0}, destination_alt = {0};

	char *string_altitude= malloc(2*sizeof(char));
	sprintf(string_altitude, "%d",altitude);
	SDL_Texture *texture_altitude=create_texture_for_text(string_altitude, graphics->font, graphics->renderer, graphics->colours->white);

	destination.w = 2 * offset_x;
	destination.h = 4 * offset_y;

	destination_alt.w = 0.5 * offset_x;
	destination_alt.h = 1.5 * offset_y;

	int i=id%39;

	if(i>=0 && i<=19){
		destination.x=2*i * offset_x + decal_x;
		destination.y=6*((int)id/39) * offset_y + decal_y;

		destination_alt.x=(2*i+0.75) * offset_x + decal_x;
		destination_alt.y=(6*((int)id/39)+2) * offset_y + decal_y;
	}
	else if (i>=20 && i<=38){
		destination.x=(((2*id)%39)) * offset_x + decal_x;
		destination.y=(6*((int)id/39)+3) * offset_y + decal_y;

		destination_alt.x=(((2*id)%39)+0.75) * offset_x + decal_x;
		destination_alt.y=(6*((int)id/39)+5) * offset_y + decal_y;
	}

	//printf("x : %d y : %d id : %d\n", destination.x,destination.y, id);

	SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(graphics->renderer, texture, &source, &destination);

	SDL_QueryTexture(texture_altitude, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(graphics->renderer, texture_altitude, &source, &destination_alt);

	SDL_DestroyTexture(texture_altitude);
	free(string_altitude);
}



void display_board_in_rect(graphics_t *g, game_t *game, board_t * board, SDL_Rect *rect) {
	(void)game;

	int offset_x=rect->w/40;
	int offset_y=rect->h/61;
	// display all cells
	for(int i=0;i<390;i++){
		display_cell_bis(g->type_texture[board->cell_tab[i]->level->cell_type], g, board->cell_tab[i]->id, board->cell_tab[i]->altitude, offset_x, offset_y, rect->x, rect->y);
	}
}




void display_tile_in_rect(SDL_Rect *rect, tile_t *tile, graphics_t *graphics){
	int offset_x = rect->w/4;
	int offset_y = rect->h/7;

	SDL_Rect source = {0}, destination = {0};

	destination.w = 2 * offset_x;
	destination.h = 4 * offset_y;

	if(tile->orientation%2==0){
		for(int i=0; i<2; i++){
			destination.x = 2 * i * offset_x + rect->x;
			destination.y = rect->y;
			SDL_QueryTexture(graphics->type_texture[tile->cell_types[(tile->orientation+1+i)%3]], NULL, NULL, &source.w, &source.h);
			SDL_RenderCopy(graphics->renderer, graphics->type_texture[tile->cell_types[(tile->orientation+1+i)%3]], &source, &destination);
		}
		destination.x = offset_x + rect->x;
		destination.y = 3 * offset_y + rect->y;
		SDL_QueryTexture(graphics->type_texture[tile->cell_types[tile->orientation%3]], NULL, NULL, &source.w, &source.h);
		SDL_RenderCopy(graphics->renderer, graphics->type_texture[tile->cell_types[tile->orientation%3]], &source, &destination);
	}
	else{
		destination.x = offset_x + rect->x;
		destination.y = rect->y;
		SDL_QueryTexture(graphics->type_texture[tile->cell_types[tile->orientation%3]], NULL, NULL, &source.w, &source.h);
		SDL_RenderCopy(graphics->renderer, graphics->type_texture[tile->cell_types[tile->orientation%3]], &source, &destination);
		for(int i=0; i<2; i++){
			destination.x = 2 * i * offset_x + rect->x;
			destination.y = 3 * offset_y + rect->y;
			SDL_QueryTexture(graphics->type_texture[tile->cell_types[(tile->orientation+2-i)%3]], NULL, NULL, &source.w, &source.h);
			SDL_RenderCopy(graphics->renderer, graphics->type_texture[tile->cell_types[(tile->orientation+2-i)%3]], &source, &destination);
		}
	}
}

void display_mouse_cells(tile_t *tile, cell_t *cell, graphics_t *graphics){
	if(tile->orientation%2){
		if(cell->neighbour[2] && cell->neighbour[1]){
			display_cell(graphics->type_texture[tile->cell_types[(tile->orientation+2)%3]], graphics, cell->id, cell->altitude+1, 0);
			display_cell(graphics->type_texture[tile->cell_types[(tile->orientation)%3]], graphics, cell->neighbour[1]->id, cell->neighbour[1]->altitude+1, 0);
			display_cell(graphics->type_texture[tile->cell_types[(tile->orientation+1)%3]], graphics, cell->neighbour[2]->id, cell->neighbour[2]->altitude+1, 0);
		}
	}
	else{
		if(cell->neighbour[0] && cell->neighbour[1]){
			display_cell(graphics->type_texture[tile->cell_types[(tile->orientation)%3]], graphics, cell->id, cell->altitude+1, 0);
			display_cell(graphics->type_texture[tile->cell_types[(tile->orientation+1)%3]], graphics, cell->neighbour[0]->id, cell->neighbour[0]->altitude+1, 0);
			display_cell(graphics->type_texture[tile->cell_types[(tile->orientation+2)%3]], graphics, cell->neighbour[1]->id, cell->neighbour[1]->altitude+1, 0);
		}
	}
}

void update_tile_position(tile_t *tile, cell_t *cell){
	if(tile->orientation%2){
		if(cell->neighbour[2] && cell->neighbour[1]){
			tile->cell_tab[(tile->orientation+2)%3]=cell;
			tile->cell_tab[(tile->orientation)%3]=cell->neighbour[1];
			tile->cell_tab[(tile->orientation+1)%3]=cell->neighbour[2];
		}
	}
	else{
		if(cell->neighbour[0] && cell->neighbour[1]){
			tile->cell_tab[(tile->orientation)%3]=cell;
			tile->cell_tab[(tile->orientation+1)%3]=cell->neighbour[0];
			tile->cell_tab[(tile->orientation+2)%3]=cell->neighbour[1];
		}
	}
}

void display_game(graphics_t* g, game_t *game){
	
	SDL_Rect source = {0};


	// clear renderer
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	SDL_RenderClear(g->renderer);


	// background
	//SDL_QueryTexture(g->background, NULL, NULL, &source.w, &source.h);
	//SDL_RenderCopy(g->renderer, g->background, &source, g->window_dimensions);


	// board
	if(game->player_board){
		display_board(g, game, game->player,0);
	}
	else {
		display_board(g, game, game->bot,0);
	}


	// panel
	SDL_SetRenderDrawColor(g->renderer, 175, 175, 175, 255);
	SDL_RenderFillRect(g->renderer, g->panel);


	// mini board
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(g->renderer, g->mini_board);
	if(game->player_board){
		display_board_in_rect(g, game, game->bot, g->mini_board);
	}
	else {
		display_board_in_rect(g, game, game->player, g->mini_board);
	}



	// text
	// "bot :"
	SDL_QueryTexture(g->bot_text, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->bot_text, &source, g->bot_rect);

	// bot score
	char *bot_score= malloc(8*sizeof(char));
	sprintf(bot_score, "%d  ",game->bot->score);
	SDL_Texture *texture_bot_score=create_texture_for_text(bot_score, g->font, g->renderer, g->colours->white);

	SDL_QueryTexture(texture_bot_score, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, texture_bot_score, &source, g->score_bot);

	SDL_DestroyTexture(texture_bot_score);
	free(bot_score);

	// "player :"
	SDL_QueryTexture(g->player_text, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->player_text, &source, g->player_rect);

	// player score
	char *player_score= malloc(8*sizeof(char));
	sprintf(player_score, "%d  ",game->player->score);
	SDL_Texture *texture_player_score=create_texture_for_text(player_score, g->font, g->renderer, g->colours->white);

	SDL_QueryTexture(texture_player_score, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, texture_player_score, &source, g->score_player);

	SDL_DestroyTexture(texture_player_score);
	free(player_score);

	// rocks
	SDL_QueryTexture(g->rocks, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->rocks, &source, g->bot_rocks);

	char *player_rocks= malloc(8*sizeof(char));
	sprintf(player_rocks, " %d  ",game->player->rocks);
	SDL_Texture *texture_player_rocks=create_texture_for_text(player_rocks, g->font, g->renderer, g->colours->white);

	SDL_QueryTexture(texture_player_rocks, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, texture_player_rocks, &source, g->n_player_rocks);

	SDL_DestroyTexture(texture_player_rocks);
	free(player_rocks);

	SDL_QueryTexture(g->rocks, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->rocks, &source, g->player_rocks);

	char *bot_rocks= malloc(8*sizeof(char));
	sprintf(bot_rocks, " %d  ",game->bot->rocks);
	SDL_Texture *texture_bot_rocks=create_texture_for_text(bot_rocks, g->font, g->renderer, g->colours->white);

	SDL_QueryTexture(texture_bot_rocks, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, texture_bot_rocks, &source, g->n_bot_rocks);

	SDL_DestroyTexture(texture_bot_rocks);
	free(bot_rocks);

	// arrows
	SDL_QueryTexture(g->left_arrow_text, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->left_arrow_text, &source, g->left_arrow);

	SDL_QueryTexture(g->right_arrow_text, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->right_arrow_text, &source, g->right_arrow);


	// tiles
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 175, 255);
	display_tile_in_rect(g->first_tile, game->card_1, g);
	display_tile_in_rect(g->second_tile, game->card_2, g);
	if(game->selected_card==1){
		SDL_RenderDrawRect(g->renderer, g->first_tile);
	}
	else if(game->selected_card==2){
		SDL_RenderDrawRect(g->renderer, g->second_tile);
	}

	// prices
	SDL_QueryTexture(g->rocks, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->rocks, &source, g->rocks_price_1);

	SDL_QueryTexture(g->text_price_1, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->text_price_1, &source, g->price_1);

	SDL_QueryTexture(g->rocks, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->rocks, &source, g->rocks_price_2);

	SDL_QueryTexture(g->text_price_2, NULL, NULL, &source.w, &source.h);
	SDL_RenderCopy(g->renderer, g->text_price_2, &source, g->price_2);


	// deck
	//if(game->deck->n>0){
		char *tiles_in_deck= malloc(17*sizeof(char));
		sprintf(tiles_in_deck, "  %d   ",34 - game->deck->n);
		SDL_Texture *texture_tiles_in_deck=create_texture_for_text(tiles_in_deck, g->font, g->renderer, g->colours->white);

		SDL_QueryTexture(texture_tiles_in_deck, NULL, NULL, &source.w, &source.h);
		SDL_RenderCopy(g->renderer, texture_tiles_in_deck, &source, g->tiles_in_deck);

		SDL_DestroyTexture(texture_tiles_in_deck);
		free(tiles_in_deck);
	//}


	// shows
	SDL_RenderPresent(g->renderer);
}
