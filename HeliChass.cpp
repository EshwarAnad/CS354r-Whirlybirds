#include "HeliChass.h"
#include "Heli.h"

HeliChass::HeliChass(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim,
    Ogre::Real scale, 
    Ogre::Real m, 
    Ogre::Vector3 pos, 
    Ogre::Real restitution, 
    Ogre::Real friction,
    Heli* p,
    Ogre::String tex = ""
    ) 
: GameObject(nym, mgr, sim, restitution, friction)
{
	sMgr = mgr;
    parent = p;
    if (mgr) {
        geom = mgr->createEntity(nym+"chassgeom", "helichassis.mesh");
        if(tex != "")
            geom->setMaterialName(tex);
        geom->setCastShadows(false);
        updateNode(nym+"chass");
        rootNode->attachObject(geom);

        // sphere starts at 100 units radius
        rootNode->scale(
            scale,
            scale,
            scale
            );

        rootNode->setPosition(pos);
    } else {
        // updateTransform aint gonna work so we have to set the transform ourselves
        tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
    }

    //need to figure this out
    shape = new btBoxShape(btVector3(scale*7.789/2, scale*4.353/2, scale*12.058/2));
    mass = m;
}

void HeliChass::updateNode(Ogre::String n){
    rootNode = rootNode->createChildSceneNode(n);
    name = n;
}

void HeliChass::updateTransform() {
    if (!rootNode) return;

    Ogre::Vector3 pos = rootNode->getParentSceneNode()->getPosition();
    tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
    Ogre::Quaternion qt = rootNode->getOrientation();
    tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
    
    if(motionState){
        motionState->updateTransform(tr);
    }
}

void HeliChass::update(){
    if (callback->ctxt.hit) {
        Ogre::String& objName = callback->ctxt.theObject->name;
		if (objName == "speed") {
			parent->setPowerup(objName);
			sMgr->destroyEntity(objName);
			sMgr->destroySceneNode(objName);
			simulator->removeObject(callback->ctxt.theObject);
		} else if (objName == "power") {
			parent->setPowerup(objName);
			sMgr->destroyEntity(objName);
			sMgr->destroySceneNode(objName);
			simulator->removeObject(callback->ctxt.theObject);
		} else if (objName == "health") {
			parent->setPowerup(objName);
			sMgr->destroyEntity(objName);
			sMgr->destroySceneNode(objName);
			simulator->removeObject(callback->ctxt.theObject);
		} else if (objName == "shield") {
			parent->setPowerup(objName);
			sMgr->destroyEntity(objName);
			sMgr->destroySceneNode(objName);
			simulator->removeObject(callback->ctxt.theObject);
		} else if(objName != parent->getPropName()) {
            if (DEBUG) { std::cout << "Hit: " << objName << std::endl; }
            hit(callback->ctxt);
        }
    }
}

void HeliChass::hit(CollisionContext& ctxt){
    parent->hit(ctxt);
}
