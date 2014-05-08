#ifndef __rocket__
#define __rocket__

#include "Simulator.h"
#include <Ogre.h>

class Heli;

class Rocket : public GameObject {
protected:
    void updateNode(Ogre::String);
    Heli* parent;
    
public:
    Rocket(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real scale, 
        Ogre::Real m, 
        Ogre::Vector3 pos, 
        Ogre::Matrix3 ax,
        float vel,
        Ogre::String
        );
    ~Rocket();

    Ogre::Vector3 pos2;
    float velocity;
    bool fired;
    bool destroy;
    Ogre::Real timeToLive;
    virtual void updateTransform(Ogre::Real delta);
	virtual void update();
    void move();
    void explode();
    void timeToExpire(Ogre::Real);
};

#endif
