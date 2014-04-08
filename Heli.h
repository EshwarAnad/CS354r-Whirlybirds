#ifndef __heli__
#define __heli__

#include "Simulator.h"
#include <Ogre.h>
#include "HeliChass.h"
#include "HeliProp.h"

class Heli {
protected:
    HeliChass* chass;
    HeliProp* prop;
    Ogre::SceneNode* rootNode;
    bool fullMove;
	Ogre::Real xTilt;
	Ogre::Real zTilt;
	Ogre::Real maxXTilt;
	Ogre::Real maxZTilt;
	float moveSpeed;
	float elevateSpeed;
	float rotSpeed;
	float levelSpeed;
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
    Ogre::SceneNode& getNode();
    GameObject* getProp();
    Ogre::Real getY();
};

#endif
