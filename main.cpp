#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL.h>
// pour que le thread de loading fonctionne il ne faut pas que le main soit le jeu (à mon avis) #include "SDL/SDL_thread.h"
#include <vector>
using namespace std;
#include <sstream>
#include <time.h>
#include "SDL_gfxPrimitives.h"

#include "constants.h"
#include "classic_function.h"
#include "Char.h"
#include "Timer.h"
#include "Math2D.h"
#include <SDL_rotozoom.h>

#include "SDL_mixer.h"


int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    stringstream caption;
    caption<<"loading map : "<<MAX<<"x"<<MAX<<" tiles size";
    SDL_WM_SetCaption( caption.str().c_str(), NULL );

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

//    SDL_EnableKeyRepeat(10,10);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 800x680 video: %s\n", SDL_GetError());
        return 1;
    }

    // load an image
    SDL_Surface* surfFond = load_image("images/fond.png");


    //Sprite from the tile sheet
    SDL_Rect clipsGround[5];
    clipsGround[0].x = 0;
    clipsGround[0].y = 0;
    clipsGround[0].w = TILE_EDGE;
    clipsGround[0].h = TILE_EDGE;

    clipsGround[1].x = 1*TILE_EDGE;
    clipsGround[1].y = 0;
    clipsGround[1].w = TILE_EDGE;
    clipsGround[1].h = TILE_EDGE;

    clipsGround[2].x = 2*TILE_EDGE;
    clipsGround[2].y = 0;
    clipsGround[2].w = TILE_EDGE;
    clipsGround[2].h = TILE_EDGE;

    clipsGround[3].x = 3*TILE_EDGE;
    clipsGround[3].y = 0;
    clipsGround[3].w = TILE_EDGE;
    clipsGround[3].h = TILE_EDGE;

    clipsGround[4].x = 4*TILE_EDGE;
    clipsGround[4].y = 0;
    clipsGround[4].w = TILE_EDGE;
    clipsGround[4].h = TILE_EDGE;

    Char perso("images/perso.png");
    Timer delta,fps;
    int frame=0;
    //TODO Créer un map avec camera comme lazy
    // Map
    srand(time(NULL));
    vector<vector<char> >world(MAX, vector<char>(MAX));
    for ( int i = 0; i < MAX; i++ )
    {
        world.push_back ( vector<char>() );
        for ( int j = 0; j < MAX; j++ )
        {
            switch(rand()%5)
            {
            case 0:
                world[i][j]=0;
                break;

            case 1:
                world[i][j]=1;
                break;

            case 2:
                world[i][j]=2;
                break;

            case 3:
                world[i][j]=3;
                break;
            }

            if(rand()%50 == 5)
                world[i][j]=4;
        }
    }

    /*music part*/
    Mix_Music *music = NULL;
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ){
        return false;
    }
    //Load the music
    music = Mix_LoadMUS( "sounds/music/forest/forest.ogg" );
    if( music == NULL ) {return false;}

    //Play the music
    if( Mix_PlayMusic( music, -1 ) == -1 ){
        return 1;;
    }
    /*end music part*/

    delta.start();
    fps.start();
    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {

            perso.handle_input(event);
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        done = true;
                        break;

                    default:
                        break;
                    }
                }
            } // end switch
        } // end of message processing

        perso.moveChar(delta.get_ticks(),world);
        delta.start();

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw bitmap

        // Evite la mémoire corrumpue
        long tempiMax = NUMB_TILE_HEIGHT+1+perso.getCameraY()/TILE_EDGE;
        long tempjMax = NUMB_TILE_WIDTH+1+perso.getCameraX()/TILE_EDGE;

        if(tempiMax>MAX)
            tempiMax=MAX;
        if(tempjMax>MAX)
            tempjMax=MAX;

        for (long i=perso.getCameraY()/TILE_EDGE;i<tempiMax;i++)
        {
            for (long j=perso.getCameraX()/TILE_EDGE;j<tempjMax;j++)
            {
                switch(world[i][j])
                {
                case 0:
                    apply_surface(TILE_EDGE*j-perso.getCameraX(),TILE_EDGE*i-perso.getCameraY(),surfFond,screen,&clipsGround[0]);
                    break;

                case 1:
                    apply_surface(TILE_EDGE*j-perso.getCameraX(),TILE_EDGE*i-perso.getCameraY(),surfFond,screen,&clipsGround[1]);
                    break;

                case 2:
                    apply_surface(TILE_EDGE*j-perso.getCameraX(),TILE_EDGE*i-perso.getCameraY(),surfFond,screen,&clipsGround[2]);
                    break;

                case 3:
                    apply_surface(TILE_EDGE*j-perso.getCameraX(),TILE_EDGE*i-perso.getCameraY(),surfFond,screen,&clipsGround[3]);
                    break;

                //TODO 4 = tree but need to convert to statics const values
                case 4:
                    apply_surface(TILE_EDGE*j-perso.getCameraX(),TILE_EDGE*i-perso.getCameraY(),surfFond,screen,&clipsGround[4]);
                    break;
                }
            }
        }
        // DRAWING ENDS HERE

        // DRAW SHADOWS
        const int NB_POINT = 5;
        short x[NB_POINT];
        short y[NB_POINT];
        int shadowSize = 2;

        for (long i=perso.getCameraY()/TILE_EDGE;i<tempiMax;i++){
            for (long j=perso.getCameraX()/TILE_EDGE;j<tempjMax;j++){
                if(world[i][j]==4){
                    int xTree = (TILE_EDGE*j-perso.getCameraX());
                    int yTree = (TILE_EDGE*i-perso.getCameraY());
                    int xPerso = perso.x+PERSO_WIDTH/2;
                    int yPerso = perso.y+PERSO_HEIGHT/2;

                    //TODO Idée : prendre les 4 segments du carré arbre et les agrandire pour ne plus s'occuper du quadrant I II III IV
                    if(xTree>xPerso && yTree<yPerso || xTree<xPerso && yTree>yPerso){
                        //Norme droite
                        double xD1 = xTree-xPerso;
                        double yD1 = yPerso-yTree;
                        Math2D::extendSegment(xD1,yD1,SCREEN_WIDTH);

                        double xD2 = xTree+TILE_EDGE-xPerso;
                        double yD2 = yPerso-yTree-TILE_EDGE;
                        Math2D::extendSegment(xD2,yD2,SCREEN_WIDTH);

                        x[0] = xTree;
                        y[0] = yTree;
                        x[1] = xTree+xD1;
                        y[1] = yTree-yD1;
                        x[2] = xTree+xD2;
                        y[2] = yTree-yD2;
                        x[3] = xTree+TILE_EDGE;
                        y[3] = yTree+TILE_EDGE;

                        if(xTree>xPerso && yTree<yPerso){
                            x[4] = xTree+TILE_EDGE;
                            y[4] = yTree;
                        }else{
                            x[4] = xTree;
                            y[4] = yTree+TILE_EDGE;
                        }
                    }else if (xTree>xPerso && yTree>yPerso || xTree<xPerso && yTree<yPerso) {//quad down right
                        double xD1 = xTree+TILE_EDGE-xPerso;
                        double yD1 = yTree-yPerso;
                        Math2D::extendSegment(xD1,yD1,SCREEN_WIDTH);

                        double xD2 = xTree-xPerso;
                        double yD2 = yTree+TILE_EDGE-yPerso;
                        Math2D::extendSegment(xD2,yD2,SCREEN_WIDTH);

                        x[0] = xTree+TILE_EDGE;
                        y[0] = yTree;
                        x[1] = xTree+xD1;
                        y[1] = yTree+yD1;
                        x[2] = xTree+xD2;
                        y[2] = yTree+yD2;
                        x[3] = xTree;
                        y[3] = yTree+TILE_EDGE;

                        if(xTree>xPerso && yTree>yPerso){
                            x[4] = xTree+TILE_EDGE;
                            y[4] = yTree+TILE_EDGE;
                        }else{
                            x[4] = xTree;
                            y[4] = yTree;;
                        }
                    }

                    filledPolygonRGBA(screen,
                    x, y,
                    NB_POINT,
                    0, 0, 0, 255);
                }
            }
        }
        // END DRAW SHADOWS

        //line between cursor and player
        lineRGBA(screen,
               perso.x+TILE_EDGE/2, perso.y+TILE_EDGE/2,
               event.motion.x, event.motion.y,
               255, 255, 255, 255);

        perso.show(screen);

        // finally, update the screen :)
        SDL_Flip(screen);

        // Calcule de FPS
        frame++;
        if( fps.get_ticks() > 1000 )
        {
            //The frame rate as a string
            std::stringstream caption;

            //Calculate the frames per second and create the string
            caption << "FPS : " << frame << " || x:" << perso.getCameraX()/TILE_EDGE + perso.getX()/TILE_EDGE << " / y: "<<perso.getCameraY()/TILE_EDGE + perso.getY()/TILE_EDGE;
            frame=0;

            //Reset the caption
            SDL_WM_SetCaption( caption.str().c_str(), NULL );

            //Restart the update timer
            fps.start();
        }
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(surfFond);

    /*CLEAN MUSIC*/
    //Free the music
//    Mix_FreeMusic( music );

    //Quit SDL_mixer
//    Mix_CloseAudio();
    /*END CLEAN MUSIC*/

    SDL_Quit();

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
