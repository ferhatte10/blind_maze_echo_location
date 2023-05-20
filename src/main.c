#include "../inc/includes.h"
#include "../inc/constants.h"
#include "../inc/header.h"
#include "3d_sound.h"


/* Declaration of Global Variables */

int WIN = 0;
int DEBUG = 0;

const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
    {0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
	{1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
	{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
	{1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

struct Player p;

int ticksLastFrame;

int main(void)
{
	SDL_Instance instance;

	/// Sound management
    initSound();

	/* Initializing the Window and Render */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n",
			SDL_GetError());
		return (1);
	}
	// init sdl_ttf
	if (TTF_Init() != 0) {
    	printf("Failed to initialize SDL TTF: %s\n", TTF_GetError());
		return (1);
	}
	/* Creating a new Window instance */
	instance.window = SDL_CreateWindow("Maze game © Copyright - Ferhat - Zakaria - Abir", SDL_WINDOWPOS_CENTERED,
					    SDL_WINDOWPOS_CENTERED,
					    WINDOW_WIDTH, WINDOW_HEIGHT, 0); // not resizable window for now
	if (instance.window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow Error: %s\n",
			SDL_GetError());
		SDL_Quit();
		return (1);
	}
	/* Create a new Renderer instance linked to the Window */
	instance.renderer = SDL_CreateRenderer(instance.window, -1,
		    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance.renderer == NULL)
	{
		SDL_DestroyWindow(instance.window);
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n",
		SDL_GetError());
		SDL_Quit();
		return (1);
	}

	/* Setting up Player */
	setupPlayer(instance);

	/* Main Game Loop */
	while (true)
	{
		SDL_RenderClear(instance.renderer);
		

		/* Retrieving Input Events */
		if (poll_events() == 1){
			system("clear");
			printf("Shame on you, you quit the game :/ \n");
			break;
		}

		/* Game Update */
		update();

		/* Rendering */
		SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 255);
		SDL_RenderClear(instance.renderer);

		// render the graphics only if the DEBUG mode is not activated
		if (DEBUG == 1){
			renderMap(instance);
			renderPlayer(instance);
		}else{
			struct textPrint tp;
    
			strcpy(tp.title, 		"Welcome to the Blind Maze Game!");
			strcpy(tp.subTitles[0], "The goal is to find the exit using the echo location.");
			strcpy(tp.subTitles[1], "Use Z (+ shift to speed up) to move forward");
			strcpy(tp.subTitles[2], "Use S (+ shift to speed up) to move backward");
			strcpy(tp.subTitles[3], "Use D (+ shift to speed up) to turn around right");
			strcpy(tp.subTitles[4], "Use Q (+ shift to speed up) to turn around left");
			strcpy(tp.subTitles[5], "Use E to hear the direction of the exit for 3sc");
			strcpy(tp.subTitles[6], "Use arrow keys to hear walls around you");
			strcpy(tp.subTitles[7], "Use H to display (toggle) the maze");
			strcpy(tp.subTitles[8], "Use ESC to quit the game");
			strcpy(tp.subTitles[9], "Copyright - Ferhat - Zakaria - Abir IAI2-2023");
			tp.numSubTexts = 10;
			renderText(instance, tp);
		}
		



	 	// get out of the game loop once the player won and render the winning screen
		if (WIN == 1){

			pauseSound(0);
			pauseSound(1);

			SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 255);
			SDL_RenderClear(instance.renderer);

			struct textPrint tp2;
    
			strcpy(tp2.title, 		"Congratulations!");
			strcpy(tp2.subTitles[0], "You found the exit!");
			strcpy(tp2.subTitles[1], "Now you can get your autograph!");
			strcpy(tp2.subTitles[2], "Copyright - Ferhat - Zakaria - Abir IAI2-2023");
			tp2.numSubTexts = 3;
			renderText(instance, tp2);
			SDL_RenderPresent(instance.renderer);

			system("clear");
			printf("You won!\n");
			sleep(4);
			break;
		}
		
		SDL_RenderPresent(instance.renderer);

	}

	/* Freeing sound Resources */
	closeSoundDevice();

	/* Render and Window destroy before quitting */
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit();

	return (0);
}
