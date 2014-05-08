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
: sim(sim)
{
    rootNode = mgr->getRootSceneNode()->createChildSceneNode(nym);
    rootNode->setPosition(pos);
    fullMove = false;
    Ogre::Vector3 org(0.0, 0.0, 0.0);
    chass = new HeliChass(nym, mgr, sim, scale, m, org, restitution, friction, this, tex);
    Ogre::Vector3 off(0.0 * scale, 5.0 * scale, 2.5 * scale);
    prop = new HeliProp(nym, mgr, sim, scale, m, off, restitution, friction, this, tex);
	
    chass->skipCollisions.push_back(prop);
    prop->skipCollisions.push_back(chass);

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
	time(&currentTime);
	sMgr = mgr;
	outOfBounds = false;
	timeToDie = 10.0;
	alive = true;
}

void Heli::DestroyAllAttachedMovableObjects( Ogre::SceneNode* i_pSceneNode )
{
   if ( !i_pSceneNode )
   {
      assert( false );
      return;
   }

   // Destroy all the attached objects
   Ogre::SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

   while ( itObject.hasMoreElements() )
   {
      Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
      i_pSceneNode->getCreator()->destroyMovableObject( pObject );
   }

   // Recurse to child SceneNodes
   Ogre::SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

   while ( itChild.hasMoreElements() )
   {
      Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
      DestroyAllAttachedMovableObjects( pChildNode );
   }
}

Heli::~Heli() {
    DestroyAllAttachedMovableObjects(rootNode);
    rootNode->removeAndDestroyAllChildren();
    sMgr->destroySceneNode(rootNode);
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
	if(alive){
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

void Heli::setPowerup(Ogre::String pwr) {
	time(&powerupTime);

	if (pwr == "speed") {
		expirePowerup();
		hasPowerup = true;
		speedModifier = 2.0;
		sim->soundSystem->playPowerUp(0);
	} else if (pwr == "power") {
		expirePowerup();
		hasPowerup = true;
		powerModifier = 2.0;
		sim->soundSystem->playPowerUp(1);
	} else if (pwr == "health") {
		health = (health + 50 > 100) ? 100 : health + 50;
		sim->soundSystem->playPowerUp(2);
	} else {
		expirePowerup();
		hasPowerup = true;
		hShield = new Ball(name+"hShield", sMgr, sim, 30.0, 1.0, sMgr->getSceneNode(name+"chass")->getPosition(), 1.0, 1.0, "Game/Shield");
		sMgr->getSceneNode(name+"hShield")->getParent()->removeChild(name+"hShield");
		rootNode->addChild(sMgr->getSceneNode(name+"hShield"));
		hShield->addToSimulator();
		hShield->setKinematic();
    	hShield->skipCollisions.push_back(prop);
		shield = true;
		sim->soundSystem->playPowerUp(3);
	}
}

void Heli::expirePowerup() {
	if (speedModifier != 1.0) {
		speedModifier = 1.0;
		sim->soundSystem->playPowerDown(0);
	} else if (powerModifier != 1.0) {
		powerModifier = 1.0;
		sim->soundSystem->playPowerDown(1);
	} else if (shield) {
		sMgr->destroyEntity(name+"hShield");
		sMgr->destroySceneNode(name+"hShield");
		sim->removeObject(hShield);
		shield = false;
		sim->soundSystem->playPowerDown(3);
	} 
	hasPowerup = false;
}

void Heli::hit(CollisionContext& ctxt, int damage, bool same){
	if(!same && !shield){
		std::cout << "Taking damage o noes" << std::endl;
		health -= damage;
	}
	if(alive && health <= 0)
		kill();
	Ogre::Vector3 temp = rootNode->getPosition();
	btVector3 spdV(xSpeed, ySpeed, zSpeed);
	btScalar mag = spdV.length();
	if(mag != 0.0 && !same){ //make sure the helicopter is moving or you will get NaN!
		//also make sure the object being collided with is not the same!
		spdV = spdV.normalize();
		//Convert speed vector to world for collision handling
		spdV = convertToWorld(spdV);
		spdV = reflect(spdV, ctxt.normal);
		//Convert back to local coords to update the speed vector of helicopter
		spdV = convertToLocal(spdV);
		spdV = mag * spdV;
		xSpeed = spdV.getX();
		ySpeed = spdV.getY();
		zSpeed = spdV.getZ();
	}
	//Push the helicopter out of the object it's colliding with by translating along the object's normal
	//This prevents the helicopter from getting stuck in the object
	rootNode->setPosition(temp + 1.0 * Ogre::Vector3(ctxt.normal.getX(), ctxt.normal.getY(), ctxt.normal.getZ()));
}

btVector3 Heli::reflect(btVector3& a, btVector3& b){
	 return btVector3(((-2 * a.dot(b)) * b) + a);
}

btVector3& Heli::convertToWorld(btVector3& a){
	Ogre::Matrix4 mat = rootNode->_getFullTransform();
	Ogre::Vector4 aOg(a.getX(), a.getY(), a.getZ(), 0);
	aOg = mat * aOg;
	a.setX(aOg.x);
	a.setY(aOg.y);
	a.setZ(aOg.z);
	return a;
}

btVector3& Heli::convertToLocal(btVector3& a){
	Ogre::Matrix4 mat = rootNode->_getFullTransform().inverse();
	Ogre::Vector4 aOg(a.getX(), a.getY(), a.getZ(), 0);
	aOg = mat * aOg;
	a.setX(aOg.x);
	a.setY(aOg.y);
	a.setZ(aOg.z);
	return a;
}

Ogre::String Heli::getChassName() {
	return chass->name;
}

Ogre::String Heli::getPropName() {
	return prop->name;
}

void Heli::inBounds(int bound, Ogre::Real dt, GUI* gui){
	Ogre::Vector3 pos = rootNode->getPosition();
	if(alive && (pos.x > bound || pos.x < -bound || pos.y < 0 || pos.z > bound || pos.z < -bound)){
		//out of bounds
		outOfBounds = true;
		timeToDie -= dt;
		if(gui != NULL){
			std::ostringstream stream;
			stream << "Return to battle or be destroyed! Time left: " << std::fixed <<  std::setprecision(1) << timeToDie;
			gui->setGameMessage(stream);
			gui->setGameMessageVisible(true);
		}

		if(alive && timeToDie <= 0)
			kill();
	}

	else if(alive && outOfBounds){
		if(gui != NULL)
			gui->setGameMessageVisible(false);
		//in bounds
		outOfBounds = false;
		timeToDie = 10.0;
		//std::cout << "Welcome back :)" << std::endl;
	}
}

void Heli::kill() {
	sim->soundSystem->playHeliExplode();	
	expirePowerup();
	alive = false;
	timeToDie = 10.0;
	timeToLive = 5.0;
	chass->setVisible(false);
	prop->setVisible(false);
	//put it in purgatory!
	rootNode->setPosition(0, -1000, 0);
	if (name != "heliAI")
		sim->soundSystem->playTaps();
}

void Heli::respawn(Ogre::Vector3 pos, Ogre::Real dt, GUI* gui){
	if(timeToLive <= 0){
		if(gui != NULL)
			gui->setGameMessageVisible(false);
		alive = true;
		health = 100;
		chass->setVisible(true);
		prop->setVisible(true);
		rootNode->setPosition(pos);
		if (name != "heliAI")
			sim->soundSystem->playTaps();
	}
	else{
		timeToLive -= dt;
		if(gui != NULL){
			std::ostringstream stream;
			stream << "Respawning in: " << std::fixed << std::setprecision(1) << timeToLive;
			gui->setGameMessage(stream);
			gui->setGameMessageVisible(true);
		}
	}
}
