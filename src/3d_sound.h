#ifndef _SOUND3D_
#define _SOUND3D_

#include "../inc/header.h"
#include "../inc/includes.h"

#define SOUND_NB 3
#define WALK_SOUND_PATH "src/wlk.wav"
#define WALL_SOUND_PATH "src/wall.wav"
#define EXIT_SOUND_PATH "src/melody.wav"

/**
 * SoundP struct represents the parameters for playing a sound.
 *
 * Attributes:
 *     soundX (float): The X absolute position of the sound source.
 *     soundY (float): The Y absolute position of the sound source.
 *     distance (float): The distance between the position of the player and the sound source.
 *     gain (float): The volume or gain of the sound.
 *     type (int): The type of the sound.
 *     playOnce (bool): Flag indicating if the sound is played only once.
 */
typedef struct SoundP {
    float soundX; // the X absolute position of the sound
    float soundY; // the Y absolute position of the sound
    float distance; // the distance between the pos of player and sound
    float gain; // the volume of the sound
    int type; // the type of the sound
    bool playOnce; // if the sound is played once
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


#endif /*_SOUND3D_*/