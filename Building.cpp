#include "Building.h"

Building::Building(
        Ogre::String rootNym,
        Ogre::String nodeNym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real worldScale,  
        Ogre::Vector3 pos,
        Ogre::Vector3 localScale,
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

    Ogre::Vector3 totalScale = localScale*worldScale;
    rootNode->setPosition(pos*worldScale);
    rootNode->scale(
       	0.01f * totalScale.x,
        0.01f * totalScale.y,
        0.01f * totalScale.z
        );
    shape = new btBoxShape(btVector3(totalScale.x/2.0,totalScale.y/2.0,totalScale.z/2.0));

}

void Building::updateNode(Ogre::String n){
    rootNode = rootNode->createChildSceneNode(n);
    name = n;
}