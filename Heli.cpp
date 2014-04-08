#include "Heli.h"

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
	maxZTilt = 15.0;
	HELI_X_SPEED = 50.0f,
	HELI_Y_SPEED = 30.0f,
	HELI_Z_SPEED = 50.0f;
}

void Heli::addToSimulator(){
    chass->addToSimulator();
    prop->addToSimulator();
}

void Heli::setKinematic(){
    chass->setKinematic();
    prop->setKinematic();
}

void Heli::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
	float xMove;
	float yMove;
	float zMove;
	float xRot = x * HELI_X_SPEED;
	float zRot = z * HELI_Z_SPEED;
	if (x < 0.0) {
		if (xTilt > -maxXTilt) {
			rootNode->roll(Ogre::Degree(-xRot), Ogre::Node::TS_LOCAL);
			xTilt -= -xRot;
		} 
	} else if (x > 0.0) {
		if (xTilt < maxXTilt) {
			rootNode->roll(Ogre::Degree(-xRot), Ogre::Node::TS_LOCAL);
			xTilt += xRot;
		}
	} else {
		/*if (xTilt < 0) {
			rootNode->roll(Ogre::Degree(x), Ogre::Node::TS_LOCAL);
			xTilt += x;
		} else {
			rootNode->roll(Ogre::Degree(-x), Ogre::Node::TS_LOCAL);
			xTilt -=x;
		}*/
	}
	if (z < 0.0) {
		if (zTilt > -maxZTilt) {
			rootNode->pitch(Ogre::Degree(zRot), Ogre::Node::TS_LOCAL);
			zTilt -= -zRot;
		} 
	} else if (z > 0.0) {
		if (zTilt < maxZTilt) {
			rootNode->pitch(Ogre::Degree(zRot), Ogre::Node::TS_LOCAL);
			zTilt += zRot;
		}
	}
	xMove = x * HELI_X_SPEED;
	yMove = y * HELI_Y_SPEED;
	zMove = z * HELI_Z_SPEED;
    rootNode->translate(rootNode->getLocalAxes(), xMove, yMove, zMove);
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
