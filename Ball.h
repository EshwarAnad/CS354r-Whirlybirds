#ifndef __ball__
#define __ball__

#include "Simulator.h"
#include <Ogre.h>

enum sounds{NOSOUND, BALLTARGET, BALL1WALL, BALL2WALL, SERVERBALLPADDLE, CLIENTBALLPADDLE};
class Ball : public GameObject {
protected:
    bool hitFlag;
    int score;
	Ogre::String nym;
	Ogre::SceneManager* sMgr;

public:
    Ball(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real radius, 
        Ogre::Real m, 
        Ogre::Vector3 pos, 
        Ogre::Real restitution, 
        Ogre::Real friction,
        Ogre::String
        );

    virtual void update();
    int& getScore();
};

#endif
