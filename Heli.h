#ifndef __heli__
#define __heli__

#include "Simulator.h"
#include <Ogre.h>
#include "HeliChass.h"
#include "HeliProp.h"
#include "Ball.h"

#define maxXZSpeed 80.0
#define maxYSpeed 50.0
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
	int health;
	float speedModifier;
	float powerModifier;
	bool shield;
	Ogre::SceneManager* sMgr;
	Simulator* si;
	Ball* hShield;

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
    void hit(CollisionContext&);
    Ogre::SceneNode& getNode();
    GameObject* getProp();
    Ogre::Real getY();
    Ogre::String getPropName();
    Ogre::String getChassName();
	void setPowerup(Ogre::String);
	void expirePowerup();
	time_t powerupTime;
	time_t currentTime;
	bool hasPowerup;
    btVector3 reflect(btVector3& a, btVector3& b);
    btVector3& convertToWorld(btVector3&);
    btVector3& convertToLocal(btVector3&);
};

#endif
