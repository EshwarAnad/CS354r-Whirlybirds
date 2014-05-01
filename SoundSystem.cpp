#include "SoundSystem.h"
#include "iostream"
#include "Ball.h"

SoundSystem::SoundSystem()
{
	isMuted = false;

    // Initialize SDL functionality using recommended settings
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    // Load the music and begin playing
    menuMusic = Mix_LoadMUS("media/sound/menuMusic.wav");
    Mix_PlayMusic(menuMusic, -1);

    // allocate 16 mixing channels
    Mix_AllocateChannels(16);

    // Load the sound effects
	rotor = Mix_LoadWAV("media/sound/rotor.wav");
	speedUp = Mix_LoadWAV("media/sound/speedUp.wav");
	speedDown = Mix_LoadWAV("media/sound/speedDown.wav");
	powerUp = Mix_LoadWAV("media/sound/powerUp.wav");
	powerDown = Mix_LoadWAV("media/sound/speedDown.wav");
	healthUp = Mix_LoadWAV("media/sound/healthUp.wav");
	shieldUp = Mix_LoadWAV("media/sound/shieldUp.wav");
	shieldDown = Mix_LoadWAV("media/sound/speedDown.wav");
}

/* mute and unmute all sounds */
void SoundSystem::mute(void)
{
	playRotor();
	isMuted = !isMuted;
}

/* Play the sounds */

void SoundSystem::playMusic(void)
{
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(menuMusic, -1);
    } else if (Mix_PausedMusic() == 1) {
        Mix_ResumeMusic();
    } else {
        Mix_PauseMusic();
    }
}

void SoundSystem::playRotor(void)
{
	if (Mix_Playing(1) == 0)
		Mix_PlayChannel(1, rotor, -1);
	else
		Mix_HaltChannel(1);
}

void SoundSystem::playPowerUp(int p)
{
	if (!isMuted && Mix_Playing(2) == 0) {
		switch (p) {
			case 0:
				Mix_PlayChannel(2, speedUp, 0);
				break;
			case 1:
				Mix_PlayChannel(2, powerUp, 0);
				break;
			case 2:
				Mix_PlayChannel(2, healthUp, 0);
				break;
			case 3:
				Mix_PlayChannel(2, shieldUp, 0);
				break;
		}
	}
}

void SoundSystem::playPowerDown(int p)
{
	if (!isMuted && Mix_Playing(2) == 0) {
		switch (p) {
			case 0:
				Mix_PlayChannel(2, speedDown, 0);
				break;
			case 1:
				Mix_PlayChannel(2, powerDown, 0);
				break;
			case 3:
				Mix_PlayChannel(2, shieldDown, 0);
				break;
		}
	}
}
