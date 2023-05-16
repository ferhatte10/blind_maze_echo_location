#ifndef _GLOBAL_
#define _GLOBAL_

#define FALSE 0
#define TRUE 1

#define TILE_SIZE 32
#define MAP_NUM_ROWS 19
#define MAP_NUM_COLS 19

#define MINIMAP_SCALE_FACTOR  1

#define WINDOW_WIDTH (MAP_NUM_COLS * TILE_SIZE)
#define WINDOW_HEIGHT (MAP_NUM_ROWS * TILE_SIZE)


#define FPS 60
#define FRAME_TIME_LENGTH (1000 / FPS)

#define FRONT 0
#define RIGHT 1
#define BACK  2
#define LEFT  3

#define yE (6 * TILE_SIZE  + (TILE_SIZE/2)) // y coordinate of the center of the exit
#define xE 0 // x coordinate of the center of the exit


#endif /* _GLOBAL_ */
