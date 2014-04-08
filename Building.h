#ifndef __building__
#define __building__

#include "Simulator.h"
#include <Ogre.h>

class Building : public GameObject {
protected:
    void updateNode(Ogre::String);
    
public:
    Building(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Vector3 scale,  
        Ogre::Vector3 pos, 
        Ogre::Real restitution, 
        Ogre::Real friction,
        Ogre::String
        );
    virtual void updateTransform();

};

#endif