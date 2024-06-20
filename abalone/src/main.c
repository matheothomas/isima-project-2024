/* name : main.c
 * author : matheo thomas, domitille vale, eloi petit
 * date : 18-06-24
 */

#include "SDL2/SDL_timer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/algos.h"
#include "../include/init.h"
#include "../include/graphics.h"
#include "../include/utilities.h"
#include "SDL2/SDL_render.h"
#include<SDL2/SDL_ttf.h>

int main(void) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_DisplayMode screen;
	SDL_Texture *board, *white, *black;

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

	TTF_Font * font=TTF_OpenFont("res/Unique.ttf", 72 );

	board_t * b = create_clean_board();
	cell_t **cell_tab=create_table(*b);

	// First Event Loop
	int h;
	int w;
	int x=0;
	int y=0;
	int r1=0;
	int r2=0;
	int r=0;
	int mouse_state=0;
	SDL_GetWindowSize(window, &w, &h);

	// Rect creation
	SDL_Rect* text_box = crea_rect(w/3, h/4, w/3, h/4);
	SDL_Rect* button_1 = crea_rect(2*w/15, 5*h/9, w/3, h/4);
	SDL_Rect* button_2 = crea_rect(8*w/15, 5*h/9, w/3, h/4);

	SDL_Rect* text_box_2 = crea_rect(13*w/18, h/11, 2*w/9, 2*h/11);
	SDL_Rect* confirm = crea_rect(7*w/9, 4*h/11, w/9, h/11);
	SDL_Rect* button = crea_rect(13*w/18, 6*h/11, 2*w/9, 2*w/9);
	
	SDL_Texture*text=create_texture_for_text("choose your\nstarting line !", font, window, renderer);
	texturing(text,window,renderer);
	SDL_RenderPresent(renderer);
	SDL_Delay(200);

	SDL_bool program_on = SDL_TRUE;
	SDL_bool program_on_2 = SDL_FALSE;
	SDL_Event event;
	while (program_on) {
		// process event
		mouse_state=0;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				program_on = SDL_FALSE;
				break;
				case SDL_MOUSEBUTTONDOWN:
				x=event.button.x;
				y=event.button.y;
				mouse_state=1;
				break;
				case SDL_MOUSEBUTTONUP:
				mouse_state=2;
				x=event.button.x;
				y=event.button.y;
				break;
				default:
				break;
			}
		}

		// update
		if(mouse_state==1){
			if(is_in(button_1, x, y)){
				r1=255;
				r2=0;
			}
			else if (is_in(button_2, x, y)){
				r1=0;
				r2=255;
			}
		}
			
		if(mouse_state==2){
			r1=0;
			r2=0;
		}

		if(mouse_state==2){
			if(is_in(button_1, x, y)){
				b=start_config(b);
				program_on = SDL_FALSE;
				program_on_2 = SDL_TRUE;
			}
			else if (is_in(button_2, x, y)){
				b=start_config_2(b);
				program_on = SDL_FALSE;
				program_on_2 = SDL_TRUE;
			}
		}
		
		// render
		home_menu(window, renderer, text_box, button_1, button_2, font, text, r1, r2,board,board);
		SDL_Delay(1);
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
			
	// Second Event Loop
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	while (program_on_2) {
		
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				program_on_2 = SDL_FALSE;
				break;

				case SDL_MOUSEBUTTONDOWN:
				x=event.button.x;
				y=event.button.y;


				if(x>7*w/9 && 7*w/9+w/9){
					if(4*h/11 && 4*h/11+h/11){
						r=255;
					}
				}
				else{
					r=0;
				}
				break;
				case SDL_MOUSEBUTTONUP:
				x=event.button.x;
				y=event.button.y;
				break;

				default:
				break;
			}
			if(x>7*w/9 && 7*w/9+w/9){
				if(4*h/11 && 4*h/11+h/11){
					program_on_2 = SDL_FALSE;
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					SDL_RenderClear(renderer);
				}
			}
			else{
				display_game(window, renderer, text_box_2, confirm, button, font, text, r, board, white, black, cell_tab);
				SDL_Delay(1);
			}
		}
	}


	// TEST FUNCTIONS

	/*
	for(int i=0;i<61;i++){
		printf("%d : %d \n",i,table[i]->state);
	}

	b -> cell -> state = BLACK;
	b -> cell -> neighbourg[0] -> state = BLACK;
	b -> cell -> neighbourg[0] -> neighbourg[0] -> state = WHITE;
	b -> cell -> neighbourg[0] -> neighbourg[0] -> neighbourg[0] -> state = BLACK;

	play_t play;
	play.cell_tab[0] = b -> cell;
	play.cell_tab[1] = b -> cell -> neighbourg[0];
	play.cell_tab[2] = b -> cell -> neighbourg[0] -> neighbourg[0];
	play.cell_tab[3] = b -> cell -> neighbourg[0] -> neighbourg[0] -> neighbourg[0];
	play.cell_tab[4] = NULL;
	play.cell_direction = 0;
	play.movement_direction = 0;
	play.cell_tab_length = 4;
	bool player = 0;

	printf("Validity play result : %d\n", validity_play(b, &play, player));
	*/ 

	// algos.c tests

	// play_t *p = malloc(sizeof(play_t));

	/*
	p->cell_tab[0] = cell_tab[13];
	p->cell_tab[1] = cell_tab[14];
	p->cell_tab[2] = cell_tab[15];
	p->buffer[0] = WHITE;
	p->buffer[1] = WHITE;
	p->buffer[2] = WHITE;
	p->cell_tab_length = 3;
	p->cell_direction = 2;
	p->movement_direction = 4;
	
	
	display_board(board, white, black, window, renderer, cell_tab);

	SDL_Delay(1000);
	b = apply_play(b, p);
	display_board(board, white, black, window, renderer, cell_tab);
	
	SDL_Delay(1000);
	b = undo_play(b, p);
	display_board(board, white, black, window, renderer, cell_tab);
	*/

	// p = choose_play(b);
	
	// b = apply_play(b, p);
	display_board(board, white, black, window, renderer, cell_tab);

	SDL_Delay(2000);

	SDL_RenderClear(renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", window, renderer);

	return 0;

}
