#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int main(int argc, char ** argv)
{
    bool quit = false;
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window * window = SDL_CreateWindow("SDL_ttf in SDL2",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,
        480, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    TTF_Font * font = TTF_OpenFont("fonts/arial.ttf", 25);
    SDL_Color color = {255, 255, 255};
    SDL_Surface * surface = TTF_RenderText_Solid(font, "Pizza Driver", color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { 0, 0, texW, texH };



    while (!quit)
    {
        SDL_WaitEvent(&event);
 
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
 
    return 0;
}