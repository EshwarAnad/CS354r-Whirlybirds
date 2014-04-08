#include "Heli.h"

extern float HELI_SPEED;

Heli::Heli(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim,
    Ogre::Real scale, 
    Ogre::Real m, 
    Ogre::Vector3 pos, 
    Ogre::Real restitution, 
    Ogre::Real friction,
    Ogre::String tex = ""
    ) 
{
    rootNode = mgr->getRootSceneNode()->createChildSceneNode(nym);
    rootNode->setPosition(pos);
    fullMove = false;

    Ogre::Vector3 org(0.0, 0.0, 0.0);
    chass = new HeliChass(nym, mgr, sim, scale, m, org, restitution, friction, tex);
    Ogre::Vector3 off(0.0 * scale, 5.0 * scale, 2.5 * scale);
    prop = new HeliProp(nym, mgr, sim, scale, m, off, restitution, friction, tex);
	xTilt = 0.0;
	zTilt = 0.0;
	maxXTilt = 25.0;
	maxZTilt = 25.0;
}

void Heli::addToSimulator(){
    chass->addToSimulator();
    prop->addToSimulator();
}

void Heli::setKinematic(){
    chass->setKinematic();
    prop->setKinematic();
}

void Heli::move(Ogre::Real x, Ogre::Real y, Ogre::Real z){
    rootNode->translate(rootNode->getLocalAxes(), x, y, z);
	if (x < 0.0) {
		if (xTilt > -maxXTilt) {
			rootNode->roll(Ogre::Degree(-x), Ogre::Node::TS_LOCAL);
			xTilt -= -x;
		} 
	} else if (x > 0.0) {
		if (xTilt < maxXTilt) {
			rootNode->roll(Ogre::Degree(-x), Ogre::Node::TS_LOCAL);
			xTilt += x;
		}
	} else {
		if (xTilt < 0) {
			rootNode->roll(Ogre::Degree(x), Ogre::Node::TS_LOCAL);
			xTilt += x;
		} else {
			rootNode->roll(Ogre::Degree(-x), Ogre::Node::TS_LOCAL);
			xTilt -=x;
		}
	}
	if (z != 0.0) {
		//if (zTilt > -maxZTilt) {
		//	rootNode->pitch(Ogre::Degree(-z), Ogre::Node::TS_LOCAL);
		//	zTilt -= z;
		//}
	}
}

void Heli::rotate(Ogre::Real angle) {
	rootNode->yaw(Ogre::Degree(angle), Ogre::Node::TS_WORLD);
}

void Heli::spin(Ogre::Real t){
    prop->spin(t);
}

void Heli::animate(Ogre::Real t){
    spin(t);
    updateTransform();
}

Ogre::SceneNode& Heli::getNode(){
    return *rootNode;
}

Ogre::Real Heli::getY(){
    return rootNode->getPosition().y;
}

GameObject* Heli::getProp(){
    return prop;
}

void Heli::setPropRot(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Real w){
    prop->getNode().setOrientation(x, y, z, w);
}

void Heli::updateTransform(){
    chass->updateTransform();
    prop->updateTransform();
}
