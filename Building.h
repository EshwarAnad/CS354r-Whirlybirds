#ifndef __building__
#define __building__

#include "Simulator.h"
#include <Ogre.h>

class Building : public GameObject {
protected:
    void updateNode(Ogre::String);
    
public:
    Building(
        Ogre::String rootNym,
        Ogre::String nodeNym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real worldScale,  
        Ogre::Vector3 pos,
        Ogre::Vector3 localScale,
        Ogre::Real restitution, 
        Ogre::Real friction,
        Ogre::String
        );

};

#endif