#include <SDL2/SDL.h>
#include "../inc/header.h"
#include "player_setup.h"

pthread_t thread;
int walking = 0;
int turning = 0;
int h_hitted = 0;

int up_key_pressed = 0;
int down_key_pressed = 0;
int left_key_pressed = 0;
int right_key_pressed = 0;
bool e_pressed = false;

/**
 * Poll SDL events and handle them.
 *
 * @return 1 if the game should quit, 0 otherwise.
 */
int poll_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return (1);
			case SDL_KEYDOWN: {
				if (event.key.keysym.sym == SDLK_ESCAPE) // quite the game
					{
						return (1);
					}
				if (event.key.keysym.sym == SDLK_z)
				{	p.walkDirection = +1;
					if (walking == 0)
					{
						walking = 1;
									
						SoundP *sound_p = malloc(sizeof(SoundP));
					
						sound_p->type = 0; // type of the sound (walk)
						sound_p->distance = 0;

						sound_p->soundX = 0;
						sound_p->soundY = 0;
						sound_p->gain = 0.5f;
						sound_p->playOnce = false;

						pthread_create(&thread, NULL, playSound, (void *) sound_p);
					}
				}
				if (event.key.keysym.sym == SDLK_s)
					{
						p.walkDirection = -1;
						if (walking == 0)
						{
							walking = 1;
							
							SoundP *sound_p = malloc(sizeof(SoundP));
						
							sound_p->type = 0; // type of the sound (walk)
							sound_p->distance = 0;

							sound_p->soundX = 0;
							sound_p->soundY = 0;
							sound_p->gain = 0.5f;
							sound_p->playOnce = false;

							pthread_create(&thread, NULL, playSound, (void *) sound_p);

						}
					}
				if (event.key.keysym.sym == SDLK_d)
					{
						p.turnDirection = +1;
						if (turning == 0)
						{
							turning = 1;
						}
					}
				if (event.key.keysym.sym == SDLK_q)
					{
						p.turnDirection = -1;
						if (turning == 0)
						{
							turning = 1;
						}
					}
				if (event.key.keysym.sym == SDLK_UP)
					{
						if (up_key_pressed == 0)
						{
							up_key_pressed = 1;
							SoundmapHasWallAtDirection(FRONT);					
						}
					}
				if (event.key.keysym.sym == SDLK_DOWN)
					{
						if (down_key_pressed == 0)
						{
							down_key_pressed = 1;
							SoundmapHasWallAtDirection(BACK);
						}
					}
				if (event.key.keysym.sym == SDLK_RIGHT)
					{
						if (right_key_pressed == 0)
						{
							right_key_pressed = 1;
							SoundmapHasWallAtDirection(RIGHT);
						}
					}
				if (event.key.keysym.sym == SDLK_LEFT)
					{
						if (left_key_pressed == 0)
						{
							left_key_pressed = 1;
							SoundmapHasWallAtDirection(LEFT);
						}
					}
				if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
				{   
					// if walking or turning
					if (walking == 1 || turning == 1)
					{
						setSpeedSound(0, 4.0f);
						p.walkSpeed = 200;
						p.turnSpeed = 80 * (M_PI / 180);
					}	
				}
				if (event.key.keysym.sym == SDLK_e)
				{
					if (!e_pressed){
						e_pressed = true;

						if (!(p.x >= TILE_SIZE-10 && p.x <= 2 * TILE_SIZE  && p.y > TILE_SIZE && p.y<= TILE_SIZE * (MAP_NUM_ROWS-1))
							 && !(p.y >= 6 * TILE_SIZE && p.y <= 7 * TILE_SIZE && p.x >= 0 && p.x <= TILE_SIZE))
						{
							
							SoundP *sound_p = malloc(sizeof(SoundP));

							sound_p->type = 2; // type of the sound (exit)

							sound_p->soundX = xE;
							sound_p->soundY = yE;
							sound_p->distance = distanceBetweenPoints(p.x, p.y, xE, yE);
							sound_p->gain = 1.0f;
							sound_p->playOnce = true;

							system("clear");
							printf("Helping you ... Hear well the direction and be smart...\n");
							pthread_t t2;
							pthread_create(&t2, NULL, playSound, (void *) sound_p);
						}
					}
					
				}
				break;
			}
			case SDL_KEYUP: {
				if (event.key.keysym.sym == SDLK_z)
				{
					p.walkDirection = 0;
					
					if (walking == 1)
					{
						walking = 0;
						pauseSound(0);
						pthread_join(thread, NULL);
					}
				}
				if (event.key.keysym.sym == SDLK_s)
					{
						p.walkDirection = 0;
						if (walking == 1)
						{
							walking = 0;
							pauseSound(0);
							pthread_join(thread, NULL);
						}
					}
				if (event.key.keysym.sym == SDLK_d)
					{
						p.turnDirection = 0;
						if (turning == 1)
						{
							turning = 0;
						}
					}
				if (event.key.keysym.sym == SDLK_q)
					{
						p.turnDirection = 0;
						if (turning == 1)
						{
							turning = 0;
						}
					}
				if (event.key.keysym.sym == SDLK_h)
					{
						// set the DEBUG variable to 1 to see the map and the player position
						if (DEBUG == 0)
							DEBUG = 1;
						else
							DEBUG = 0;
					}
				if (event.key.keysym.sym == SDLK_UP)
					{
						up_key_pressed = 0;
					}						
				if (event.key.keysym.sym == SDLK_DOWN)
					{
						down_key_pressed = 0;
					}
				if (event.key.keysym.sym == SDLK_RIGHT)
					{
						right_key_pressed = 0;
					}
				if (event.key.keysym.sym == SDLK_LEFT)
					{
						left_key_pressed = 0;
					}
				if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
				{						
					setSpeedSound(0, 2.0f);
					p.walkSpeed = 100;
					p.turnSpeed = 45 * (M_PI / 180);
				}
				if (event.key.keysym.sym == SDLK_e)
				{
					e_pressed = false;
				}
				break;
			}
		}
	}
	return (0);
}