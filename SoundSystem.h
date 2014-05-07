/*
 * Handles sound output using the SDl library.
 */

#ifndef __SoundSystem_h_
#define __SoundSystem_h_

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_audio.h>

class SoundSystem
{
protected:
    Mix_Music *menuMusic;
	Mix_Chunk *rotor;
	Mix_Chunk *speedUp;
	Mix_Chunk *speedDown;
	Mix_Chunk *powerUp;
	Mix_Chunk *powerDown;
	Mix_Chunk *healthUp;
	Mix_Chunk *shieldUp;
	Mix_Chunk *shieldDown;
	Mix_Chunk *shootRocket;
	Mix_Chunk *wallHit;
	Mix_Chunk *heliExplode;
	Mix_Chunk *rocketExplode;
	Mix_Chunk *taps;

public:
	bool isMuted;
    SoundSystem(void);
	void mute(void);
    void playMusic(void);
	void playRotor(void);
	void playPowerUp(int);
	void playPowerDown(int);
	void playShootRocket(void);
	void playWallHit(void);
	void playHeliExplode(void);
	void playRocketExplode(void);
	void playTaps(void);
};

#endif //#ifndef __SoundSystem_h_
