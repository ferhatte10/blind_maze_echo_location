#ifndef _HEADER_
#define _HEADER_

#include <SDL2/SDL.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "constants.h"

extern const int map[MAP_NUM_ROWS][MAP_NUM_COLS];

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int isGameRunning;


typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

struct Player
{
	float x;
	float y;
	float width;
	float height;
	int turnDirection;
	int walkDirection;
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
};
extern struct Player p;


typedef struct textPrint {
    char title[100];
    char subTitles[10][100];
	int  numSubTexts;

} textPrint;

extern int ticksLastFrame;
extern int WIN;
extern int walking;
extern int DEBUG;
extern int deltaTime;

/* Functions */
int   init_instance(SDL_Instance *);
int   poll_events(void);
void  renderMap(SDL_Instance instance);
void  setupPlayer();
void  renderPlayer(SDL_Instance instance);
void  renderText(SDL_Instance instance, struct textPrint tp);
void  movePlayer(float deltaTime);
int   mapHasWallAt(float x, float y);
void  update(void);
float distanceBetweenPoints(float x1, float y1, float x2, float y2);

#endif /* _HEADER_ */
