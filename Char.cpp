#include "Char.h"

#include "constants.h"
#include "classic_function.h"
#include <SDL_rotozoom.h>
#include "SDL_Image.h"
#define PI 3.14159265

Char::Char(string filename)
{
    upPress=downPress=leftPress=rightPress=false;
    cameraX=cameraY=x=y=0;
    velocity=2;
    xVel=yVel=0;
    delta=0;
    lastMove=LAST_DOWN;
    surf = IMG_Load(filename.c_str());
    angle=0;

}

Char::~Char()
{
    SDL_FreeSurface(surf);
}

void Char::handle_input(SDL_Event &event)
{
    // Rotation handling
    if(event.type==SDL_MOUSEMOTION){
        double yTemp = -(y+TILE_EDGE/2-event.motion.y);
        double xTemp = x+TILE_EDGE/2-event.motion.x;
        if(xTemp!=0){//avoid div 0
            angle = atan(yTemp/xTemp)*180/PI;
            angle += ((xTemp<0?0:180));
        }
    }// Move handling
    else if(event.type==SDL_KEYDOWN){
        switch(event.key.keysym.sym)
        {
        case SDLK_UP: yVel -= SPEED_PERSO; upPress=true; break;
        case SDLK_DOWN: yVel += SPEED_PERSO; downPress=true; break;
        case SDLK_LEFT: xVel -= SPEED_PERSO; leftPress=true; break;
        case SDLK_RIGHT: xVel += SPEED_PERSO; rightPress=true; break;
        default: break;
        }
    }
    else if(event.type==SDL_KEYUP)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_UP: yVel += SPEED_PERSO; upPress=false; break;
        case SDLK_DOWN: yVel -= SPEED_PERSO; downPress=false; break;
        case SDLK_LEFT: xVel += SPEED_PERSO; leftPress=false; break;
        case SDLK_RIGHT: xVel -= SPEED_PERSO; rightPress=false; break;
        default: break;
        }
    }
}

void Char::show(SDL_Surface *screen)
{
    SDL_Surface *surfRotate = rotozoomSurface(surf, angle, 1.0, 0);
    int deltaX = (surf->w-surfRotate->w)/2;
    int deltaY = (surf->h-surfRotate->h)/2;
    apply_surface(x+deltaX,y+deltaY,surfRotate,screen);
}

//--------------------------------------
//Getter
//--------------------------------------

int Char::getX(){return x;}
int Char::getY(){return y;}
long Char::getCameraX(){return cameraX;}
long Char::getCameraY(){return cameraY;}

//--------------------------------------
// End Getter
//--------------------------------------

//--------------------------------------
// Move -------------------------------
//--------------------------------------

void Char::moveChar(Uint32 deltaTicks,vector<vector<char> > &world)
{
    delta+=deltaTicks;
    if(delta>1)
    {
        if(setCameraX(world))
        {
            x += xVel;

            //If the dot went too far to the left or right or touched a wall
            if(( x < 0 ) || ( x + PERSO_WIDTH > SCREEN_WIDTH ) || touchWall(world))
            {
                //move back
                x -= xVel;
            }
        }

        if(setCameraY(world))
        {
            y += yVel;

            if(( y < 0 ) || ( y + PERSO_HEIGHT > SCREEN_HEIGHT ) || touchWall(world))
            {
                //move back
                y -= yVel;
            }
        }

        delta=0;
    }
}

bool Char::setCameraY(vector<vector<char> > &world)
{
    //TODO Attention +1 à enlever une fois que les collisions sont propre !
    if((cameraY<=0 && y<SCREEN_HEIGHT/2) || (y>=SCREEN_HEIGHT/2 && cameraY+SCREEN_HEIGHT+1>=MAX*TILE_EDGE ))
    {
        return true;
    }
    else
    {
        cameraY+=yVel;

        if(touchWall(world))
        {
            cameraY-=yVel;
        }
        if(cameraY<0 || cameraY+SCREEN_HEIGHT>=MAX*TILE_EDGE)
        {
            cameraY-=yVel;
            return true;
        }
        return false;
    }
}

bool Char::setCameraX(vector<vector<char> > &world)
{
    //TODO Attention +1 à enlever une fois que les collisions sont propre !
    if(( cameraX<=0 && x<SCREEN_WIDTH/2) || (x>=SCREEN_WIDTH/2 && cameraX+SCREEN_WIDTH+1>=MAX*TILE_EDGE) )
    {
        return true;
    }
    else
    {
        cameraX+=xVel;

        if(touchWall(world))
        {
            cameraX-=xVel;
        }
        if(cameraX<0 || cameraX+SCREEN_WIDTH>=MAX*TILE_EDGE)
        {
            cameraX-=xVel;
            return true;
        }
        return false;
    }
}

bool Char::touchWall(vector<vector<char> > &world)
{
    // Evite la mémoire corrumpue
    long tempiMax = NUMB_TILE_HEIGHT+1+cameraY/TILE_EDGE;
    long tempjMax = NUMB_TILE_WIDTH+1+cameraX/TILE_EDGE;

    if(tempiMax>MAX)
        tempiMax=MAX;
    if(tempjMax>MAX)
        tempjMax=MAX;

    for (long i=cameraY/TILE_EDGE;i<tempiMax;i++)
    {
        for (long j=cameraX/TILE_EDGE;j<tempjMax;j++)
        {
            if(world[i][j]==4)
            {
                //TODO 30x30 pas du tout propre ça devrait être 32x32
                if(collision(x,y,30,30,TILE_EDGE*j-cameraX,TILE_EDGE*i-cameraY,32,32))
                    return true;//tile touche it
            }

        }
    }
    return false;//no tile touche it
}
