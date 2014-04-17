#ifndef __heli__
#define __heli__

#include "Simulator.h"
#include <Ogre.h>
#include "HeliChass.h"
#include "HeliProp.h"

#define maxXZSpeed 60.0
#define maxYSpeed 30.0
#define maxYawSpeed 30.0
#define speedIncrement 0.05
#define speedBase 0.002
#define maxTilt 25.0
#define levelSpeed 0.02
#define rotSpeed 20.0

class Heli {
protected:
    HeliChass* chass;
    HeliProp* prop;
    Ogre::SceneNode* rootNode;
    bool fullMove;
	Ogre::Real xTilt;
	Ogre::Real zTilt;
	float xSpeed;
	float ySpeed;
	float zSpeed;
	float yawSpeed;
	Ogre::String name;
	float speedModifier;

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
	void speedPowerup();
};

#endif
