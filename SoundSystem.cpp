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
	powerDown = Mix_LoadWAV("media/sound/powerDown.wav");
	healthUp = Mix_LoadWAV("media/sound/healthUp.wav");
	shieldUp = Mix_LoadWAV("media/sound/shieldUp.wav");
	shieldDown = Mix_LoadWAV("media/sound/shieldDown.wav");
	shootRocket = Mix_LoadWAV("media/sound/rocket.wav");
	wallHit = Mix_LoadWAV("media/sound/wallHit.wav");
	heliExplode = Mix_LoadWAV("media/sound/heliExplode.wav");
	Mix_VolumeChunk(heliExplode, 128);
	rocketExplode = Mix_LoadWAV("media/sound/rocketExplode.wav");
	taps = Mix_LoadWAV("media/sound/taps.wav");
	Mix_VolumeChunk(taps, 50);
}

SOUND SoundSystem::getLastSoundPlayed() {
    SOUND ret = lastSoundPlayed;
    lastSoundPlayed = SOUND_NONE;
    return ret;
}

void SoundSystem::playSound(SOUND s) {
    switch (s) {
        case SOUND_ROCKET:
            playShootRocket();
        case SOUND_ROCKET_EXPLODE:
            playRocketExplode();
        case SOUND_HELI_EXPLODE:
            playHeliExplode();
        case SOUND_WALL_HIT:
            playWallHit();
        break; 
    }
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
	if (!isMuted && Mix_Playing(3) == 0) {
		switch (p) {
			case 0:
				Mix_PlayChannel(3, speedDown, 0);
				break;
			case 1:
				Mix_PlayChannel(3, powerDown, 0);
				break;
			case 3:
				Mix_PlayChannel(3, shieldDown, 0);
				break;
		}
	}
}

void SoundSystem::playShootRocket()
{
   lastSoundPlayed = SOUND_ROCKET;
    
	if (!isMuted) {
		if (Mix_Playing(4) == 0)
			Mix_PlayChannel(4, shootRocket, 0);
		else if (Mix_Playing(5) == 0)
			Mix_PlayChannel(5, shootRocket, 0);
	}
}

void SoundSystem::playWallHit()
{
    lastSoundPlayed = SOUND_WALL_HIT;

	if (!isMuted && Mix_Playing(6) == 0)
		Mix_PlayChannel(6, wallHit, 0);
}

void SoundSystem::playHeliExplode()
{
    lastSoundPlayed = SOUND_HELI_EXPLODE;
	
    if (!isMuted && Mix_Playing(7) == 0)
		Mix_PlayChannel(7, heliExplode, 0);
}

void SoundSystem::playRocketExplode()
{
    lastSoundPlayed = SOUND_ROCKET_EXPLODE;
	
    if (!isMuted) {
		if (Mix_Playing(8) == 0)
			Mix_PlayChannel(8, rocketExplode, 0);
		else if (Mix_Playing(9) == 0)
			Mix_PlayChannel(9, rocketExplode, 0);
	}
}

void SoundSystem::playTaps()
{
	std::cout << "TAPS!";
	if (!isMuted) {
		if (Mix_Playing(10) == 0) {
			Mix_PlayChannel(10, taps, 0);
			playRotor();
		} else {
			Mix_HaltChannel(10);
			playRotor();
		}
	}
}
