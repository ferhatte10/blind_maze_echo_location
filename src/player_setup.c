#include "player_setup.h"

// define a macro for converting degrees to radians
#define DEG_TO_RAD(x) ((x) * M_PI / 180.0f)

int is_near = 0;

int hit = 0;

pthread_t t_wall;


void setupPlayer()
{
    p.x = TILE_SIZE + TILE_SIZE/2 ;
    p.y = TILE_SIZE * 17 + TILE_SIZE/2  ;
    p.width = 10;
    p.height = 10;
    p.turnDirection = 0;
    p.walkDirection = 0;
    p.rotationAngle = -(M_PI / 2);
    p.walkSpeed = 100;
    p.turnSpeed = 45 * (M_PI / 180);
}

void renderPlayer(SDL_Instance instance)
{
    SDL_SetRenderDrawColor(instance.renderer, 255, 255, 255, 255);
    
    SDL_Rect playerRect = {
        p.x * MINIMAP_SCALE_FACTOR,
        p.y * MINIMAP_SCALE_FACTOR,
        p.width * MINIMAP_SCALE_FACTOR,
        p.height * MINIMAP_SCALE_FACTOR
    };
    
    SDL_RenderFillRect(instance.renderer, &playerRect);

    SDL_RenderDrawLine(
        instance.renderer,
        
        MINIMAP_SCALE_FACTOR * p.x + p.width/2,
        MINIMAP_SCALE_FACTOR * p.y + p.height/2,
        MINIMAP_SCALE_FACTOR * (p.x + cos(p.rotationAngle) * 30),
        MINIMAP_SCALE_FACTOR * (p.y + sin(p.rotationAngle) * 30)
    );
}


int mapHasWallAt(float x, float y)
{
    int mapGridIndexX, mapGridIndexY;

    // Check if the position is outside the map boundaries
    if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
        return (FALSE);
    }

    // Calculate the grid indices corresponding to the position
    mapGridIndexX = floor(x / TILE_SIZE);
    mapGridIndexY = floor(y / TILE_SIZE);

    // Check if the map grid cell at the calculated indices contains a wall (non-zero value)
    return (map[mapGridIndexY][mapGridIndexX] != 0);
}


void movePlayer(float deltaTime)
{
    float moveStep, newPlayerX, newPlayerY;

    p.rotationAngle += p.turnDirection * p.turnSpeed * 1.10 * deltaTime;

    moveStep = p.walkDirection * p.walkSpeed * deltaTime;

    newPlayerX = p.x + cos(p.rotationAngle) * moveStep;
    newPlayerY = p.y + sin(p.rotationAngle) * moveStep;
    
    float yE = 6 * TILE_SIZE  + (TILE_SIZE/2); // y coordinate of the center of the exit
    float xE =  0 ;// x coordinate of the center of the exit

    // float ySeuil = 8 * TILE_SIZE + (TILE_SIZE/2);
    // float xSeuil = 1 * TILE_SIZE + (TILE_SIZE/2);

    float distancePES = distanceBetweenPoints(newPlayerX, newPlayerY, xE, yE); // distance between player and exit
    // float distanceSeuil = distanceBetweenPoints(xE, yE,xSeuil, ySeuil);

    bool is_in_coridor = newPlayerX > TILE_SIZE - 5 && newPlayerX < 2 * TILE_SIZE + 5 && newPlayerY > TILE_SIZE + 5 && newPlayerY <= TILE_SIZE * (MAP_NUM_ROWS-1);
    bool is_in_exit_tail = (p.y >= 6 * TILE_SIZE && p.x <= 7 * TILE_SIZE && p.x >= 0 && p.x <= TILE_SIZE);
    bool won = (p.y >= 6 * TILE_SIZE && p.x <= 7 * TILE_SIZE && p.x >= 0 && p.x <= TILE_SIZE/2);
    
    if (is_in_coridor || is_in_exit_tail) // if player is in the coridor near the exit
    {
        SoundP *sound_p = malloc(sizeof(SoundP));
          
        sound_p->type = 2; // type of the sound (exit)

        sound_p->soundX = xE;
        sound_p->soundY = yE;
        sound_p->distance = distancePES;
        sound_p->gain = 1.0f;       

        if (!is_near)
        {
            system("clear");
            printf("You're pretty good you approched the exit\n");
            is_near = 1;
            pthread_t t2;
            pthread_create(&t2, NULL, playSound, (void *) sound_p);

        }
        else{
            // set the sound position
            set3dPositionSound(2, xE, yE, distancePES);
        }
    }
    else {
        if (is_near)
        {
            is_near = 0;
            system("clear");
            printf("You're getting away from the exit\n");
            pauseSound(2);  
        }
    }
    
    


    if (!mapHasWallAt(newPlayerX, newPlayerY))
    {

        p.x = newPlayerX;
        p.y = newPlayerY;
        
        if (won) // if player enters to the exit case
        {
            WIN = 1;
        }

        hit = 0;   

    } else {
        //thread to play soun wall
        if (hit == 0)
        {
            hit = 1;
            SoundP *sound_w = malloc(sizeof(SoundP));
          
            sound_w->type = 1; // type of the sound (wall)

            sound_w->distance = 1.5*distanceBetweenPoints(p.x, p.y, newPlayerX, newPlayerY); // distance between player (listener) and sound source (wall) * 2 to have a better effect of the sound because the distance is too small

            sound_w->soundX = newPlayerX;
            sound_w->soundY = newPlayerY;
           

            sound_w->gain = 1.0f;


			pthread_create(&t_wall, NULL, playSound, (void *) sound_w);
            pauseSound(0);

        }
    }
}

void SoundmapHasWallAtDirection(int direction){

    float angle = p.rotationAngle;
    float moveStep, newPlayerX, newPlayerY;
    

    for (int i = 0; i <= TILE_SIZE; i += moveStep) {
        
        moveStep = 1.0;  // Adjust the step size as needed

        if (direction == FRONT) {
            newPlayerX = p.x + cos(angle) * i;
            newPlayerY = p.y + sin(angle) * i;
        }
        else if (direction == RIGHT) {
            newPlayerX = p.x + cos(angle + M_PI/2) * i;
            newPlayerY = p.y + sin(angle + M_PI/2) * i;
        }
        else if (direction == BACK) {
            newPlayerX = p.x + cos(angle + M_PI) * i;
            newPlayerY = p.y + sin(angle + M_PI) * i;
        }
        else if (direction == LEFT) {
            newPlayerX = p.x + cos(angle - M_PI/2) * i;
            newPlayerY = p.y + sin(angle - M_PI/2) * i;
        }
  
        if (mapHasWallAt(newPlayerX, newPlayerY)) {
            
            SoundP *sound_w = malloc(sizeof(SoundP));
          
            sound_w->type = 1; // type of the sound (wall)

            sound_w->distance = distanceBetweenPoints(p.x, p.y, newPlayerX, newPlayerY); // distance between player (listener) and sound source (wall) * 2 to have a better effect of the sound because the distance is too small

            sound_w->soundX = newPlayerX;
            sound_w->soundY = newPlayerY;
           

            sound_w->gain = 1.0f;


			pthread_create(&t_wall, NULL, playSound, (void *) sound_w);
            break;
        }
    }

}