#include "Building.h"

Building::Building(
        Ogre::String rootNym,
        Ogre::String nodeNym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real worldScale,  
        Ogre::Vector3 pos,
        Ogre::Vector3 dim,
        Ogre::Vector3 rot,
        Ogre::Real restitution, 
        Ogre::Real friction,
        Ogre::String tex
        ) : GameObject(rootNym, mgr, sim, restitution, friction){

	geom = mgr->createEntity(nodeNym, "cube.mesh");
    if(tex != "")
        geom->setMaterialName(tex);
    geom->setCastShadows(false);
    updateNode(nodeNym);
    rootNode->attachObject(geom);

    Ogre::Vector3 actualDim = dim*worldScale;
    rootNode->setPosition(pos*worldScale);
    rootNode->pitch(Ogre::Radian(rot.x));
    rootNode->yaw(Ogre::Radian(rot.y));
    rootNode->roll(Ogre::Radian(rot.z));
    rootNode->scale(
       	0.01f * actualDim.x,
        0.01f * actualDim.y,
        0.01f * actualDim.z
        );
    shape = new btBoxShape(btVector3(actualDim.x/2.0,actualDim.y/2.0,actualDim.z/2.0));

}

void Building::updateNode(Ogre::String n){
    rootNode = rootNode->createChildSceneNode(n);
    name = n;
}