#ifndef __helichass__
#define __helichass__

#include "Simulator.h"
#include <Ogre.h>

class Heli;

class HeliChass : public GameObject {
protected:
    void updateNode(Ogre::String);
    Heli* parent;
    
public:
    HeliChass(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real scale, 
        Ogre::Real m, 
        Ogre::Vector3 pos, 
        Ogre::Real restitution, 
        Ogre::Real friction,
        Heli* p,
        Ogre::String
        );
    virtual void updateTransform();
    virtual void update();
    void hit();

};

#endif