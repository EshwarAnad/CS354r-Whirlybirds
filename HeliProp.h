#ifndef __heliprop__
#define __heliprop__

#include "Simulator.h"
#include <Ogre.h>

class HeliProp : public GameObject {
protected:
    void updateNode(Ogre::String);

public:
    HeliProp(
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
    virtual void updateTransform();
    void spin(Ogre::Real);
};

#endif