#include "Wall.h"

Wall::Wall(Ogre::String nym, 
	Ogre::SceneManager* mgr, 
	Simulator* sim, 
	Ogre::Real x, 
	Ogre::Real y, 
	Ogre::Real z, 
	Ogre::Real width, 
	Ogre::Real height,
	Ogre::Vector3 normal,
    Ogre::Vector3 texVect,
    Ogre::Real restitution,
    Ogre::Real friction,
    Ogre::String tex = "") 
: GameObject(nym, mgr, sim, restitution, friction) 
{
    //setup Ogre
	if (mgr) {
        Ogre::Plane sceneWall(normal, 0);

        Ogre::MeshManager::getSingleton().createPlane(nym, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        sceneWall, width, height, 20, 20, true, 1, 5, 5, texVect);

        geom = mgr->createEntity(nym + "Entity", nym);
        rootNode->attachObject(geom);
        rootNode->setPosition(x, y, z);

        if(tex != "")
            geom->setMaterialName(tex);
        geom->setCastShadows(false);
    }

    if(normal == Ogre::Vector3::UNIT_Y || normal == -Ogre::Vector3::UNIT_Y)
        shape = new btBoxShape(btVector3(width/2, 5.0, height/2));
    else if(normal == Ogre::Vector3::UNIT_X || normal == -Ogre::Vector3::UNIT_X)
        shape = new btBoxShape(btVector3(5.0, width/2, height/2));
    else if(normal == Ogre::Vector3::UNIT_Z || normal == -Ogre::Vector3::UNIT_Z)
        shape = new btBoxShape(btVector3(width/2, height/2, 5.0));
    else
        shape = NULL;
}

void Wall::update() {
    if (callback->ctxt.hit) {
        if (simulator->soundOn) {
            if (isBall(callback->ctxt.theObject->name)) {
                //simulator->soundSystem->playWallHit();
            }
        }
    }
}
