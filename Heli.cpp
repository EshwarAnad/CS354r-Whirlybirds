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
	xTilt = 0.0;
	zTilt = 0.0;
	xSpeed = 0.0;
	zSpeed = 0.0;
	ySpeed = 0.0;
	speedModifier = 1.0;
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
	float xRot = x * rotSpeed;
	float zRot = z * rotSpeed;

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
}

void Heli::hit(CollisionContext& ctxt){
	std::cout << "Taking damage o noes" << std::endl;
	Ogre::Vector3 temp = rootNode->getPosition();
	//Need to get speed based on world coords
	btVector3 spdV(xSpeed, ySpeed, zSpeed);
	btScalar mag = spdV.length();
	spdV = spdV.normalize();
	Ogre::Quaternion rot = rootNode->getOrientation();
	Ogre::Vector3 eulerRot(rot.getRoll().valueRadians(), rot.getPitch().valueRadians(), rot.getYaw().valueRadians());
	std::cout << "Rotation = (" << eulerRot.x << ", " << eulerRot.y << ", " << eulerRot.z << ")" << std::endl;
	std::cout << "velocity before Rotation = (" << spdV.getX() << ", " << spdV.getY() << ", " << spdV.getZ() << ")" << std::endl;
	spdV = convertToWorld(spdV, rot);
	std::cout << "velocity before = (" << spdV.getX() << ", " << spdV.getY() << ", " << spdV.getZ() << ")" << std::endl;
	std::cout << "wall normal = (" << ctxt.normal.getX() << ", " << ctxt.normal.getY() << ", " << ctxt.normal.getZ() << ")" << std::endl;
	spdV = reflect(spdV, ctxt.normal);
	std::cout << "velocity after = (" << spdV.getX() << ", " << spdV.getY() << ", " << spdV.getZ() << ")" << std::endl;
	rootNode->setPosition(temp + Ogre::Vector3(spdV.getX(), spdV.getY(), spdV.getZ()));
	//Convert back to local coords
	spdV = convertToLocal(spdV, rot);
	std::cout << "velocity after Rotation = (" << spdV.getX() << ", " << spdV.getY() << ", " << spdV.getZ() << ")" << std::endl;
	spdV = mag * spdV;
	xSpeed = spdV.getX();
	ySpeed = spdV.getY();
	zSpeed = spdV.getZ();

}

void Heli::updateTime(Ogre::Real& t){
	prop->updateTime(t);
	chass->updateTime(t);
}

btVector3 Heli::reflect(btVector3& a, btVector3& b){
	 return btVector3(((-2 * a.dot(b)) * b) + a);
}

btVector3& Heli::convertToWorld(btVector3& a, Ogre::Quaternion& rot){
	Ogre::Matrix4 mat = rootNode->_getFullTransform();
	//Ogre::Vector3 eulerRot(rot.getPitch().valueRadians(), rot.getYaw().valueRadians(), rot.getRoll().valueRadians());
	/*Ogre::Matrix4 xMat = rotXMatrix(-eulerRot.x);
	Ogre::Matrix4 yMat = rotYMatrix(-eulerRot.y);
	Ogre::Matrix4 zMat = rotZMatrix(-eulerRot.z);*/
	Ogre::Vector4 aOg(a.getX(), a.getY(), a.getZ(), 0);
	/*aOg = xMat * aOg;
	aOg = yMat * aOg;
	aOg = zMat * aOg;*/
	aOg = mat * aOg;
	a.setX(aOg.x);
	a.setY(aOg.y);
	a.setZ(aOg.z);
	return a;
}

btVector3& Heli::convertToLocal(btVector3& a, Ogre::Quaternion& rot){
	Ogre::Matrix4 mat = rootNode->_getFullTransform().inverse();
	/*Ogre::Vector3 eulerRot(rot.getPitch().valueRadians(), rot.getYaw().valueRadians(), rot.getRoll().valueRadians());
	Ogre::Matrix4 xMat = rotXMatrix(eulerRot.x);
	Ogre::Matrix4 yMat = rotYMatrix(eulerRot.y);
	Ogre::Matrix4 zMat = rotZMatrix(eulerRot.z);*/
	Ogre::Vector4 aOg(a.getX(), a.getY(), a.getZ(), 0);
	/*aOg = zMat * aOg;
	aOg = yMat * aOg;
	aOg = xMat * aOg;*/
	aOg = mat * aOg;
	a.setX(aOg.x);
	a.setY(aOg.y);
	a.setZ(aOg.z);
	return a;
}

Ogre::Matrix4 Heli::rotXMatrix(Ogre::Real rot){
	return Ogre::Matrix4(1, 0, 		  0, 		 0,
						 0, cos(rot), -sin(rot), 0,
						 0, sin(rot), cos(rot),  0,
						 0, 0,		  0,		 1);
}

Ogre::Matrix4 Heli::rotYMatrix(Ogre::Real rot){
	return Ogre::Matrix4(cos(rot),  0, sin(rot), 0,
						 0, 	    1, 0, 	     0,
						 -sin(rot), 0, cos(rot), 0,
						 0, 0,		  0,		 1);
	
}

Ogre::Matrix4 Heli::rotZMatrix(Ogre::Real rot){
	return Ogre::Matrix4(cos(rot), -sin(rot), 0, 0,
						 sin(rot), cos(rot),  0, 0,
						 0, 	   0, 		  1, 0,
						 0, 	   0,		  0, 1);
	
}