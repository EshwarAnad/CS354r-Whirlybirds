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
    chass = new HeliChass(nym, mgr, sim, scale, m, org, restitution, friction, this, tex);
    Ogre::Vector3 off(0.0 * scale, 5.0 * scale, 2.5 * scale);
    prop = new HeliProp(nym, mgr, sim, scale, m, off, restitution, friction, this, tex);
    Ogre::Vector3 rock(0.0 * scale, 10.0 * scale, 0 * scale);
    //rocket = new Rocket(nym, mgr, sim, scale, m, rock, restitution, friction, this, tex);
	xTilt = 0.0;
	zTilt = 0.0;
	xSpeed = 0.0;
	zSpeed = 0.0;
	ySpeed = 0.0;
	yawSpeed = 0.0;
	speedModifier = 1.0;
	powerModifier = 1.0;
	shield = false;
	name = nym;
	health = 100.0;
	hasPowerup = false;
	time(&powerupTime);
}

void Heli::addToSimulator() {
    chass->addToSimulator();
    prop->addToSimulator();
    //rocket->addToSimulator();
}

void Heli::setKinematic(){
    chass->setKinematic();
    prop->setKinematic();
}

void Heli::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
	float xMove;
	float yMove;
	float zMove;
	float xRot = x * rotSpeed;
	float zRot = z * rotSpeed;

	//check if powerup time has elapsed
	time(&currentTime);
	if (hasPowerup && difftime(currentTime, powerupTime) >= 30)
		expirePowerup();
	
	if (x < 0.0) {
		if (xSpeed > 0.0) {
			x = -x;
			xSpeed -= speedIncrement;
		} else if (xSpeed > -maxXZSpeed)
			xSpeed -= speedIncrement;

		if (xTilt > -maxTilt) {
			rootNode->roll(Ogre::Degree(-xRot), Ogre::Node::TS_LOCAL);
			xTilt -= -xRot;
		} 
	} else if (x > 0.0) {
		if (xSpeed < 0.0) {
			x = -x;
			xSpeed += speedIncrement;
		} else if (xSpeed < maxXZSpeed)
			xSpeed += speedIncrement;

		if (xTilt < maxTilt) {
			rootNode->roll(Ogre::Degree(-xRot), Ogre::Node::TS_LOCAL);
			xTilt += xRot;
		}
	} else {
		if (xSpeed < 0.0) {
			x = -speedBase;
			xSpeed += speedIncrement;
		} else if (xSpeed > 0.0) {
			x = speedBase;
			xSpeed -= speedIncrement;
		}

		if (xTilt < 0.0) {
			rootNode->roll(Ogre::Degree(-levelSpeed), Ogre::Node::TS_LOCAL);
			xTilt += levelSpeed;
		} else if (xTilt > 0.0) {
			rootNode->roll(Ogre::Degree(levelSpeed), Ogre::Node::TS_LOCAL);
			xTilt -= levelSpeed;
		}
	}

	if (z < 0.0) {
		if (zSpeed > 0.0) {
			z = -z;
			zSpeed -= speedIncrement;
		} else if (zSpeed > -maxXZSpeed)
			zSpeed -= speedIncrement;

		if (zTilt > -maxTilt) {
			rootNode->pitch(Ogre::Degree(zRot), Ogre::Node::TS_LOCAL);
			zTilt -= -zRot;
		} 
	} else if (z > 0.0) {
		if (zSpeed < 0.0) {
			z = -z;
			zSpeed += speedIncrement;
		} else if (zSpeed < maxXZSpeed)
			zSpeed += speedIncrement;

		if (zTilt < maxTilt) {
			rootNode->pitch(Ogre::Degree(zRot), Ogre::Node::TS_LOCAL);
			zTilt += zRot;
		}
	} else {
		if (zSpeed < 0.0) {
			z = -speedBase;
			zSpeed += speedIncrement;
		} else if (zSpeed > 0.0) {
			z = speedBase;
			zSpeed -= speedIncrement;
		}

		if (zTilt < 0.0) {
			rootNode->pitch(Ogre::Degree(levelSpeed), Ogre::Node::TS_LOCAL);
			zTilt += levelSpeed;
		} else if (zTilt > 0.0) {
			rootNode->pitch(Ogre::Degree(-levelSpeed), Ogre::Node::TS_LOCAL);
			zTilt -= levelSpeed;
		}
	}

	if (y < 0.0) {
		if (ySpeed > 0.0) {
			ySpeed -= speedIncrement;
			y = -y;
		} else if (ySpeed > -maxYSpeed)
			ySpeed -= speedIncrement;
	} else if (y > 0.0) {
		if (ySpeed < 0.0) {
			ySpeed += speedIncrement;
			y = -y;
		} else if (ySpeed < maxYSpeed)
			ySpeed += speedIncrement;
	} else {
		if (ySpeed < 0.0) {
			y = -speedBase;
			ySpeed += speedIncrement;
		} else if (ySpeed > 0.0) {
			y = speedBase;
			ySpeed -= speedIncrement;
		}
	}

	xMove = x * std::abs(xSpeed) * speedModifier;
	yMove = y * std::abs(ySpeed) * speedModifier;
	zMove = z * std::abs(zSpeed) * speedModifier;
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
    //rocket->updateTransform();
}

void Heli::hit(){
	if (DEBUG) {
        std::cout << "Taking damage o noes" << std::endl;
    }
}

void Heli::setPowerup(Ogre::String pwr) {
	time(&powerupTime);

	hasPowerup = true;
	if (pwr == "speed") {
		expirePowerup();
		speedModifier = 3.0;
	} else if (pwr == "power") {
		expirePowerup();
		powerModifier = 2.0;
	} else if (pwr == "health") {
		hasPowerup = false;
		health = (health + 50 > 100) ? 100 : health + 50;
	} else {
		expirePowerup();
		shield = true;
	}
}

void Heli::expirePowerup() {
	speedModifier = 1.0;
	powerModifier = 1.0;
	shield = false;
	hasPowerup = false;
}

void Heli::fire() {
	//rocket = new Rocket(nym, mgr, sim, scale, m, rock, restitution, friction, this, tex);
	//rocket->fired = 1;
	//rocket->addToSimulator();
	//rocket->setKinematic();
}
