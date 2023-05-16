#include "3d_sound.h"

// Declare global variables for OpenAL resources

ALCdevice*  device;
ALCcontext* context;


// declare a list of sound sources and buffers
ALuint     sources[SOUND_NB]; // 0 = walking, 1 = wall, 2 = exit
ALuint     buffers[SOUND_NB]; // 0 = walking, 1 = wall, 2 = exit

float      SoundX, SoundY;


void fatalError(char *state){
    printf("%s: %d", state, alGetError());
    exit(1);
}

void initDeviceAndContext() {
    device = alcOpenDevice(NULL);

    if (!device)
        fatalError("OpenAL Error: No sound device detected");

    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    if (!context)
        fatalError("OpenAL Error: No sound context");
}


void loadSound(const char* filename, ALuint *buffer, ALuint *source) {
    // Create buffer
    alGenBuffers(1, buffer);

    // Load WAV data from file
    FILE* file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    char* data = (char*)malloc(size);
    fread(data, size, 1, file);
    fclose(file);

    // Get format, frequency, and data from WAV file
    ALenum format;
    if (strstr(filename, ".wav")) {
        format = AL_FORMAT_MONO16;
    } else {
        format = AL_FORMAT_STEREO16;
    }
    ALsizei freq = 44100;

    // Transfer data to OpenAL buffer
    alBufferData(*buffer, format, data, size, freq);

    // Free memory used by data
    free(data);

    // Create source and attach buffer
    alGenSources(1, source);
    alSourcei(*source, AL_BUFFER, *buffer);
}

void initSound() {

    initDeviceAndContext();
    // Load the sound files
    loadSound(WALK_SOUND_PATH, &buffers[0], &sources[0]);
    loadSound(WALL_SOUND_PATH, &buffers[1], &sources[1]);
    loadSound(EXIT_SOUND_PATH, &buffers[2], &sources[2]);
}

void closeSoundDevice() {    

    // Clean up the OpenAL resources we allocated
    for (int i = 0; i < SOUND_NB; i++) {
        alDeleteSources(1, &sources[i]);
        alDeleteBuffers(1, &buffers[i]);
    }

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
    alutExit();
}

void *playSound(void* arg) {
    
    SoundP* sound_p = (SoundP*) arg;
    
    // set the distance model to inverse distance
    alDistanceModel(AL_INVERSE_DISTANCE);

    // set the position of the sound source by default
    ALfloat sourcePos[3] = { 0.0f,   0.0f,   0.0f };

    if (sound_p->type == 1 || sound_p->type == 2) // wall configuration or exit configuration
    {
        set3dPositionSound(sound_p->type, sound_p->soundX, sound_p->soundY, sound_p->distance);
    }
    else {
        alSourcefv( sources[sound_p->type], AL_POSITION, sourcePos );
    }

    /// Listener
    // Orientation of the listener, needed for sound calculs
    ALfloat ListenerOri[] = { 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 1.0f };
    alListenerfv(AL_ORIENTATION, ListenerOri);

    
    alSourcef(sources[sound_p->type], AL_GAIN, sound_p->gain); // set the gain (volume) of the sound



    if (sound_p->type == 0 ) {
        alSourcef(sources[sound_p->type], AL_PITCH, 2.0f);
    } else if (sound_p->type == 1) {
        alSourcef(sources[sound_p->type], AL_PITCH, 5.0f);
    }
    
  
    if ((sound_p->type == 0 || sound_p->type == 2)) {
        // set the sound to loop
        alSourcei(sources[sound_p->type], AL_LOOPING, AL_TRUE);
    } 
    
    

    // play the sound
    alSourcePlay(sources[sound_p->type]);

    if(sound_p->playOnce){
        // play the sound for 3 seconds then pause it
        sleep(3);
        alSourcePause(sources[sound_p->type]);
    }
    

    // wait for the sound to finish playing
    if ( sound_p->type == 1)
    {
        ALint state;
        do {
            alGetSourcei(sources[sound_p->type], AL_SOURCE_STATE, &state);
        } while (state == AL_PLAYING);
        pthread_exit(NULL);
    }

    free(sound_p);
    

}



void stopSound(int type) {
    // stop the specified sound source
    alSourceStop(sources[type]);
}

void pauseSound(int type)
{
    alSourcePause(sources[type]);
}

void resumeSound(int type)
{
    // if the source is not playin resume it
    ALint state;
    alGetSourcei(sources[type], AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
        alSourcePlay(sources[type]);
}

void setSpeedSound(int type, float speed)
{
    alSourcef(sources[type], AL_PITCH, speed);
}


void updateSoundPos(float x, float y, float distance){
    //Calculate the distance between the player and the exit:
    /*
    1 Calcul de la distance entre le joueur et la sortie :
        dx représente la différence horizontale entre la position de la sortie et celle du joueur.
        dy représente la différence verticale entre la position de la sortie et celle du joueur.
    */
    float dx = x - p.x; // distance between the player and the exit
    float dy = y - p.y; // distance between the player and the exit

    //Calculate the direction from the player to the exit:
    /*
    2 Calcul de la direction du joueur vers la sortie :
        dirX et dirY correspondent à la normalisation du vecteur formé par dx et dy. Cela signifie que les valeurs sont ajustées pour représenter une direction unitaire, indépendamment de la distance réelle entre le joueur et la sortie.
    */
    float dirX = dx / distance; // normalize the vector
    float dirY = dy / distance; // normalize the vector

    /*
    3 Calcul de l'angle entre le joueur et la sortie :

        anglePE est calculé en utilisant la fonction acos() pour trouver l'arc cosinus de dirX, qui représente la projection horizontale de la direction du joueur vers la sortie. La condition dirY < 0 permet de gérer les angles dans les quadrants supérieurs.
        L'angle anglePE est utilisé pour déterminer l'angle du son en ajoutant l'angle de rotation du joueur (p.rotationAngle) et π/2 pour compenser l'orientation de l'écran (le coin supérieur gauche étant le point de référence).
    */
    float anglePE = dirY < 0 ? acos(dirX) : 2 * M_PI - acos(dirX); // angle between the player and the exit

    float angleSound = anglePE + p.rotationAngle + M_PI / 2; // angle of the sound (inversed cause of the screen start at the top left corner)

    // rotate the position according to the player's orientation
    /*
    3 Rotation de la position du son en fonction de l'orientation du joueur :
        SoundX et SoundY représentent les nouvelles coordonnées de la source sonore après avoir tourné les coordonnées initiales selon l'angle angleSound.
        Ces coordonnées sont calculées en utilisant les fonctions trigonométriques cos() et sin() respectivement, multipliées par la distance entre le joueur et la sortie.
    */
    SoundX = cos(angleSound) * distance;
    SoundY = sin(angleSound) * distance;
}

void set3dPositionSound(int type, float x, float y, float distance)
{
    updateSoundPos(x, y, distance);
    alSource3f(sources[type], AL_POSITION, SoundX, SoundY, 0.0f);
}