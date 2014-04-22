#ifndef __heli__
#define __heli__

#include "Simulator.h"
#include <Ogre.h>
#include "HeliChass.h"
#include "HeliProp.h"

#define maxXZSpeed 60.0
#define maxYSpeed 30.0
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
    void hit(CollisionContext&);
    void updateTime(Ogre::Real&);
    Ogre::SceneNode& getNode();
    GameObject* getProp();
    Ogre::Real getY();
    btVector3 reflect(btVector3& a, btVector3& b);
    btVector3& convertToWorld(btVector3&, Ogre::Quaternion&);
    btVector3& convertToLocal(btVector3&, Ogre::Quaternion&);
    Ogre::Matrix4 rotXMatrix(Ogre::Real rot);
    Ogre::Matrix4 rotYMatrix(Ogre::Real rot);
    Ogre::Matrix4 rotZMatrix(Ogre::Real rot);
};

#endif
