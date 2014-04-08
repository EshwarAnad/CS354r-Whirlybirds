#include "Level.h"

int MAPSIZE = 512;

Level::Level(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real scale, 
        Ogre::Vector3 pos, 
        Ogre::Real restitution, 
        Ogre::Real friction,
        Ogre::String tex
        ){

	rootNode = mgr->getRootSceneNode()->createChildSceneNode(nym);
    rootNode->setPosition(pos);

	base = new Wall(nym, "base", mgr, sim, 0.0, 0.0, 0.0, scale*MAPSIZE, scale*MAPSIZE, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, restitution, friction, tex);

	Ogre::Vector3 bPos(6.23, 77.05, -10.08);
	Ogre::Vector3 bScale(39.27, 154.09, 39.27);
	building = new Building(nym, "centerBuilding", mgr, sim, scale, bPos, bScale, restitution, friction, "");


}
void Level::addToSimulator(){
	base->addToSimulator();
	building->addToSimulator();
}
void Level::updateTransform(){
	base->updateTransform();
	building->updateTransform();
}
Ogre::SceneNode& Level::getNode(){
	return *rootNode;
}