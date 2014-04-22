#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Ball.h"
#include "Target.h"


Ball::Ball(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim,
    Ogre::Real radius, 
    Ogre::Real m, 
    Ogre::Vector3 pos, 
    Ogre::Real restitution, 
    Ogre::Real friction,
    Ogre::String tex = ""
    ) 
: GameObject(nym, mgr, sim, restitution, friction)
{
	sMgr = mgr;
    if (mgr) {
        geom = mgr->createEntity(name, "sphere.mesh");
        if(tex != "")
            geom->setMaterialName(tex);
        geom->setCastShadows(false);
        rootNode->attachObject(geom);

        // sphere starts at 100 units radius
        rootNode->scale(
            radius * 0.01f,
            radius * 0.01f,
            radius * 0.01f
            );

        rootNode->setPosition(pos);
    } else {
        // updateTransform aint gonna work so we have to set the transform ourselves
        tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
    }

    shape = new btSphereShape(radius);
    mass = m;
    hitFlag = false;
    score = 0;
}

void Ball::update() {
    if (callback->ctxt.hit) {
        Ogre::String& objName = callback->ctxt.theObject->name;
    } else {
        //nothing was hit
    }
}

int& Ball::getScore(){
    return score;
}
