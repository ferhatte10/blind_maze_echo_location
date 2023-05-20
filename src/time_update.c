#include "../inc/header.h"

/**
 * Update the game state.
 * Return: void
 */
void update(void)
{
    float deltaTime;

    while (
        !SDL_TICKS_PASSED(SDL_GetTicks(),
        ticksLastFrame + FRAME_TIME_LENGTH)
    );

    deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
    ticksLastFrame = SDL_GetTicks();
    movePlayer(deltaTime);
    
}
