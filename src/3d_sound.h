#include "../inc/header.h"
#include "../inc/includes.h"

#define SOUND_NB 3
#define WALK_SOUND_PATH "src/wlk.wav"
#define WALL_SOUND_PATH "src/wall.wav"
#define EXIT_SOUND_PATH "src/melody.wav"

typedef struct SoundP {
    float soundX; // the X absolute position of the sound
    float soundY; // the Y absolute position of the sound
    float distance; // the distance between the pos of player and sound
    float gain; // the volume of the sound
    int type; // the type of the sound
} SoundP;

void fatalError(char *state);
void initDeviceAndContext();
void closeSoundDevice();
void initSound();


void loadSound(const char* filename, ALuint *buffer, ALuint *source);
void *playSound(void* arg);
void stopSound(int type);
void pauseSound(int type);
void resumeSound(int type);
void setSpeedSound(int type, float speed);
void updateSoundPos(float x, float y, float distance);
void set3dPositionSound(int type, float x, float y, float distance);