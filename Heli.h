#ifndef __heli__
#define __heli__

#include "Simulator.h"
#include <Ogre.h>
#include "HeliChass.h"
#include "HeliProp.h"
#include "Rocket.h"

#define maxXZSpeed 180.0
#define maxYSpeed 90.0
#define maxYawSpeed 30.0
#define speedIncrement 1.0
#define speedBase 0.002
#define maxTilt 25.0
#define levelSpeed 0.02
#define rotSpeed 20.0

class Heli {
protected:
    HeliChass* chass;
    HeliProp* prop;
    //Rocket* rocket;
    Ogre::SceneNode* rootNode;
    bool fullMove;
	Ogre::Real xTilt;
	Ogre::Real zTilt;
	float xSpeed;
	float ySpeed;
	float zSpeed;
	float yawSpeed;
	Ogre::String name;
	int health;
	float speedModifier;
	float powerModifier;
	bool shield;

public:
    Heli(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real scale, 
        Ogre::Real m, 
        Ogre::Vector3 pos, 
        Ogre::Real restitution, 
        Ogre::Real friction,
        Ogre::String
        );
    //Rocket* rocket;
    void addToSimulator();
    void setKinematic();
    void move(Ogre::Real, Ogre::Real, Ogre::Real);
    void spin(Ogre::Real);
    void animate(Ogre::Real);
    void updateTransform();
    void setPropRot(Ogre::Real, Ogre::Real, Ogre::Real, Ogre::Real);
	void rotate(Ogre::Real);
    void hit();
    Ogre::SceneNode& getNode();
    GameObject* getProp();
    Ogre::Real getY();
	void setPowerup(Ogre::String);
	void expirePowerup();
    void fire();
	time_t powerupTime;
	time_t currentTime;
	bool hasPowerup;
};

#endif
