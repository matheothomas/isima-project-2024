#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include "X_fenêtré.h"

SDL_Window   * creafenetre(int x,int y){
	SDL_Window * window=SDL_CreateWindow("fenetre", x, y, 42, 42, SDL_WINDOW_BORDERLESS);
    if (!SDL_INIT_VIDEO){
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
	}
	return window;
}

int main(){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0,&dm);
    int h=dm.h;
    int w=dm.w;
    
    for(int i=0;i<h-72;i++){
        SDL_Window * window_1=creafenetre(0+i,0+i);
        SDL_Delay(1);
        SDL_Window * window_2=creafenetre(10+i,10+i);
        SDL_Delay(2);
        SDL_Window * window_3=creafenetre(20+i,20+i);
        SDL_Delay(3);
        SDL_Window * window_4=creafenetre(30+i,30+i);
        SDL_Delay(4);
        SDL_DestroyWindow(window_1);
        SDL_DestroyWindow(window_2);
        SDL_DestroyWindow(window_3);
        SDL_DestroyWindow(window_4);
    }
    for(int i=0;i<w-h+72;i++){
        SDL_Window * window_1=creafenetre(h-72+i,h-42-i);
        SDL_Delay(2);
        SDL_Window * window_2=creafenetre(h-62+i,h-52-i);
        SDL_Delay(4);
        SDL_Window * window_3=creafenetre(h-52+i,h-62-i);
        SDL_Delay(6);
        SDL_Window * window_4=creafenetre(h-42+i,h-72-i);
        SDL_Delay(8);
        SDL_DestroyWindow(window_1);
        SDL_DestroyWindow(window_2);
        SDL_DestroyWindow(window_3);
        SDL_DestroyWindow(window_4);
    }

    SDL_Quit();
    return 0;
}

