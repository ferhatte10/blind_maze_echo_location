#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include "../inc/header.h"
#include "../inc/constants.h"
#include "3d_sound.h"


void setupPlayer();
void renderPlayer(SDL_Instance instance);
int  mapHasWallAt(float x, float y);
void movePlayer(float deltaTime);
void SoundmapHasWallAtDirection(int direction);
