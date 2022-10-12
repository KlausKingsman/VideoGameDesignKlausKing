#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;

void init();
void loadMedia();
void close();


SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* carback = NULL;
SDL_Surface* bhouse = NULL;
SDL_Surface* rhouse = NULL;
SDL_Surface* road = NULL;

void init(){
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		gScreenSurface = SDL_GetWindowSurface( gWindow );	
}

void loadMedia(){
	carback = SDL_LoadBMP("img/Carbackblue.bmp");
    bhouse = SDL_LoadBMP("img/HouseBlue.bmp");
    rhouse = SDL_LoadBMP("img/HouseRed.bmp");
    road = SDL_LoadBMP("img/road.bmp");
}

void close(){
	SDL_FreeSurface(carback);
    SDL_FreeSurface(bhouse);
    SDL_FreeSurface(rhouse);
    SDL_FreeSurface(road);
	carback = NULL;
    bhouse = NULL;
    rhouse = NULL;
    road = NULL;
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	SDL_Quit();
}

int main( int argc, char* args[] ){
    srand(time(NULL));
    SDL_Rect dest;
    SDL_Rect dest2;
    int posx = 0;
    int posy = 0;
    int swap = 0;
    int gps = (rand()%118+2)&(-2);
    int destination = (rand()%118+2)&(-2);
    while(gps>=destination-20 && gps<=destination+20) gps=(rand()%118+2)&(-2);
    init();
    loadMedia();
    for(int i=0;i<120;i++){
        dest.x = posx;
        dest.y = posy;
        dest.w = 50;
        dest.h = 50;
        dest2.x = posx+50;
        dest2.y = posy;
        dest2.w = 50;
        dest2.h = 50;
        if(i%2==swap ){
            if(i>destination-1 && i<destination+2){
            SDL_BlitScaled( rhouse, NULL, gScreenSurface, &dest );
            SDL_BlitScaled( road, NULL, gScreenSurface, &dest2 );
            }else if(i==gps){
            SDL_BlitScaled( bhouse, NULL, gScreenSurface, &dest );
            SDL_BlitScaled( carback, NULL, gScreenSurface, &dest2 );
            }else{
            SDL_BlitScaled( bhouse, NULL, gScreenSurface, &dest );
            SDL_BlitScaled( road, NULL, gScreenSurface, &dest2 );
            }
        }else{
            if(i==gps){
            SDL_BlitScaled( carback, NULL, gScreenSurface, &dest );
            SDL_BlitScaled( road, NULL, gScreenSurface, &dest2 );
            }else{
            SDL_BlitScaled( road, NULL, gScreenSurface, &dest );
            SDL_BlitScaled( road, NULL, gScreenSurface, &dest2 );    
            }
        }
        posx = posx + 100;
        if(i%10==9){
            posy=posy+50;
            posx=0;
            if(swap == 0){
                swap++;
            }else{
                swap=0;
            }
        }
    }
    SDL_UpdateWindowSurface( gWindow );
    SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
	close();
	return 0;
}