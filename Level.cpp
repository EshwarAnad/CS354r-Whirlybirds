#include "Level.h"

extern const int MAPSIZE;
extern const int NUMBUILDINGS;

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

	Ogre::String name;
	Ogre::Vector3 bPos;
	Ogre::Vector3 bDim;
	Ogre::Vector3 bRot;
	//read from file to get positions, dimensions and rotation of buildings
	std::string fileName = "LevelInfo.txt";
	std::ifstream data;
	openFile(fileName, data);
	for(int i = 0; i < NUMBUILDINGS; i++){
		name = getName(data);
		bPos = getData(data);
		bDim = getData(data);
		bRot = getData(data);
		debug(name, bPos, bDim, bRot);
		building[i] = new Building(nym, name, mgr, sim, scale, bPos, bDim, bRot, restitution, friction, "");
	}
	data.close();

}
void Level::addToSimulator(){
	base->addToSimulator();
	for(int i = 0; i < NUMBUILDINGS; i++)
		building[i]->addToSimulator();
}
void Level::updateTransform(){
	base->updateTransform();
	for(int i = 0; i < NUMBUILDINGS; i++)
		building[i]->updateTransform();
}
Ogre::SceneNode& Level::getNode(){
	return *rootNode;
}

Ogre::String Level::getName(std::ifstream& data){
	std::string name;
	std::getline(data, name);
	return name;
}

Ogre::Vector3 Level::getData(std::ifstream& data){
	Ogre::String info;
	std::getline(data,info);
	std::stringstream ss(info);
	std::string temp;
	std::getline(ss,temp, ' ');
	Ogre::Real x = atof(temp.c_str());
	std::getline(ss,temp, ' ');
	Ogre::Real y = atof(temp.c_str());
	std::getline(ss,temp, ' ');
	Ogre::Real z = atof(temp.c_str());
	Ogre::Vector3 bPos(x, y, z);
	return bPos;
}

void Level::openFile(std::string& name, std::ifstream& data){
	data.open(name.c_str());
	if(!data.is_open()){
		std::cout << "Failed to open file" << std::endl;
		assert(0);
	}
}

void Level::debug(Ogre::String& name, Ogre::Vector3& bPos, Ogre::Vector3& bDim, Ogre::Vector3& bRot){
	std::cout << "Name = " << name << std::endl;
	std::cout << "@ (" << bPos.x << ", " << bPos.y << ", " << bPos.z << ")" << std::endl;
	std::cout << "Dim = (" << bDim.x << ", " << bDim.y << ", " << bDim.z << ")" << std::endl;
	std::cout << "Rot = (" << bRot.x << ", " << bRot.y << ", " << bRot.z << ")" << std::endl; 
	std::cout << std::endl;
}