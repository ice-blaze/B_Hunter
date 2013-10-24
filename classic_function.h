#ifndef CLASSIC_FUNCTION_H
#define CLASSIC_FUNCTION_H

#include "SDL.h"
#include <string>

typedef struct rectDragSelect{
    Sint16 x, y;
    Uint16 w, h;
    int mouse;//UP or DOWN
}rectDragSelect;

int MyRand(int a, int b);
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip=NULL);
SDL_Surface *load_image( std::string filename );
Uint32 get_pixel32( int x, int y, SDL_Surface *surface );
void waitInput(SDL_Event event);
bool collision (int x1,int y1, int w1, int h1,
               int x2, int y2, int w2, int h2);
bool collision(SDL_Rect r1,SDL_Rect r2);
bool collision(SDL_Rect r,int x,int y, int w, int h);
SDL_Rect convDrag2Rect(rectDragSelect r);

#endif
