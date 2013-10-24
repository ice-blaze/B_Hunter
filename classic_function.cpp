#include "classic_function.h"
#include <time.h>//pour faire de vrai rand aléatoir
#include <cstdlib>
#include "SDL_image.h"

int MyRand(int a, int b){
	srand(time(NULL));//pour faire de vrai rand aléatoir
    return rand()%(++b-a) +a;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    //On blitte la surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

SDL_Surface *load_image( std::string filename )
{
    //L'image qui est chargée
    SDL_Surface* loadedImage = NULL;

    //L'image optimisée qu'on va utiliser
    SDL_Surface* optimizedImage = NULL;

    //Chargement de l'image
    loadedImage = IMG_Load( filename.c_str() );

    //Si l'image a bien chargée
    if( loadedImage != NULL )
    {
        //Création de l'image optimisée
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Libération de l'ancienne image
        SDL_FreeSurface( loadedImage );

        //Si la surface a bien été optimisée
        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //On retourne l'image optimisée
    return optimizedImage;
}

Uint32 get_pixel32( int x, int y, SDL_Surface *surface )
{
    //Convertie les pixels en 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Recupere le pixel demande
    return pixels[ ( y * surface->w ) + x ];
}

void waitInput(SDL_Event event)
{
    int quit = false;
    while( quit == false )
    {
        //Tant qu'il y a un événement
        while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN)
			{
				switch(event.key.keysym.sym)
				{
                    //Sinon warning
                    default :
                        quit=true;
                        break;
				}
			}

            //Si l'utilisateur a cliqué sur le X de la fenêtre
            if( event.type == SDL_QUIT )
            {
                //On quitte the programme
                quit = true;
                SDL_Quit();
            }
        }
    }
}

bool collision (int x1,int y1, int w1, int h1,
               int x2, int y2, int w2, int h2)
{
    if(x1>x2+w2
       ||x1+w1<x2
       ||y1>y2+h2
       ||y1+h1<y2)
       {
           return false;
       }
   else
   {
       return true;
   }
}

bool collision(SDL_Rect r1,SDL_Rect r2)
{
    if(r1.x>r2.x+r2.w
       ||r1.x+r1.w<r2.x
       ||r1.y>r2.y+r2.h
       ||r1.y+r1.h<r2.y)
       {
           return false;
       }
   else
   {
       return true;
   }
}

bool collision(SDL_Rect r,int x,int y, int w, int h)
{
    if(r.x>x+w
       ||r.x+r.w<x
       ||r.y>y+h
       ||r.y+r.h<y)
       {
           return false;
       }
   else
   {
       return true;
   }
}

SDL_Rect convDrag2Rect(rectDragSelect r)
{
    SDL_Rect ret;
    ret.x=r.x;
    ret.y=r.y;
    ret.w=r.w;
    ret.h=r.h;
    return ret;
}
