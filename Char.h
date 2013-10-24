#ifndef CHAR_H
#define CHAR_H

#include "SDL.h"
#include <string>
#include <vector>
using namespace std;

const int LAST_UP = 0;
const int LAST_DOWN = 1;
const int LAST_LEFT = 2;
const int LAST_RIGHT = 3;

class Char
{
    public:
        Char(string filename);
        ~Char();

        int x,y;

        void handle_input(SDL_Event &event);
        void show(SDL_Surface *screen);

        void moveChar(Uint32 deltaTicks,vector<vector<char> > &world);

        int getX();
        int getY();
        long getCameraX();
        long getCameraY();

    protected:
    private:
        SDL_Surface *surf;
        long cameraX,cameraY;
        int velocity;
        int xVel, yVel;
        long delta;
        int lastMove;
        double angle;

        bool upPress;
        bool downPress;
        bool leftPress;
        bool rightPress;

        bool touchWall(vector<vector<char> > &world);
        bool setCameraY(vector<vector<char> > &world);
        bool setCameraX(vector<vector<char> > &world);
};

#endif // CHAR_H
