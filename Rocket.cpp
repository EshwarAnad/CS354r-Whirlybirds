#include "Rocket.h"
#include "Heli.h"

Rocket::Rocket(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim,
    Ogre::Real scale, 
    Ogre::Real m,
    Ogre::Vector3 pos,
    Ogre::Matrix3 ax,
    float vel,
    Ogre::String tex = ""
    ) 
: GameObject(nym, mgr, sim, restitution, friction)
{
    pos2 = pos;
    velocity = vel;
    if (mgr) {
        geom = mgr->createEntity(nym+"chassgeom", "fish.mesh");
        if(tex != "")
            geom->setMaterialName(tex);
        geom->setCastShadows(false);
        updateNode(nym+"chassgeom");
        rootNode->attachObject(geom);
        //rootNode->showBoundingBox(true);
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
    shape = new btBoxShape(btVector3(scale/2, scale*2, scale/2));
    mass = m;
    fired = 0;
}

//not used
void Rocket::move(){//(Ogre::Real x, Ogre::Real y, Ogre::Real z){
    float xMove = 0.0;
    float yMove = 0;
    float zMove = -5.0;

    rootNode->translate(rootNode->getLocalAxes(), xMove, yMove, zMove);

}

void Rocket::explode(){
    fired = 0;
    //simulator->removeObject("rocket");
    std::cout << "explode" << std::endl;
}

void Rocket::updateNode(Ogre::String n){
    rootNode = rootNode->createChildSceneNode(n);
    name = n;
}

void Rocket::updateTransform(Ogre::Real delta) {
    if (!rootNode) return;


    //Ogre::Vector3 pos = rootNode->getParentSceneNode()->getPosition();
    tr.setOrigin(btVector3(pos2.x, pos2.y, pos2.z));
    Ogre::Quaternion qt = rootNode->getOrientation();
    tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
    

    if(motionState){
        motionState->updateTransform(tr);
    }


}

void Rocket::update() {
	if (callback->ctxt.theObject != NULL) {
		Ogre::String& objName = callback->ctxt.theObject->name;
		if (callback->ctxt.hit) {
			 if (Ogre::StringUtil::startsWith(objName, "cube", true) || objName == "base") {
				simulator->soundSystem->playRocketExplode();
			}
		}
	}
}
