#ifndef __level__
#define __level__

#include "Simulator.h"
#include <Ogre.h>
#include "Building.h"

class Level {
protected:
    Building* building;
    Ogre::SceneNode* rootNode;
public:
    Level(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Vector3 scale, 
        Ogre::Vector3 pos, 
        Ogre::Real restitution, 
        Ogre::Real friction,
        Ogre::String
        );
    void addToSimulator();
    void updateTransform();
    Ogre::SceneNode& getNode();
};

#endif