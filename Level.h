#ifndef __level__
#define __level__

#include "Simulator.h"
#include <Ogre.h>
#include "Building.h"
#include "Wall.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <string>

const int NUMBUILDINGS = 30;
const int MAPSIZE = 512;

class Level {
protected:
    Building* building[NUMBUILDINGS];
    Wall* base;
    Ogre::SceneNode* rootNode;
    void openFile(std::string& name, std::ifstream& data);
    Ogre::Vector3 getData(std::ifstream& data);
    Ogre::String getName(std::ifstream& data);
    void debug(Ogre::String&, Ogre::Vector3&, Ogre::Vector3&, Ogre::Vector3&);
public:
    Level(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real scale, 
        Ogre::Vector3 pos, 
        Ogre::Real restitution, 
        Ogre::Real friction,
        Ogre::String
        );
    void addToSimulator();
    void updateTransform();
    Ogre::SceneNode& getNode();
    int getBounds();
};

#endif