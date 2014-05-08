#include "GameObject.h"
#include "Simulator.h"
#include <exception>

GameObject::GameObject(Ogre::String nym, Ogre::SceneManager* mgr, Simulator* sim, Ogre::Real res, Ogre::Real fric){
	name = nym;
	sceneMgr = mgr;
	simulator = sim;
	restitution = res;
	friction = fric;
	
    if (sceneMgr) {
        try{
            rootNode = mgr->getSceneNode(name);
        }
        catch (std::exception& e) {
            rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
        }
    } else {
        rootNode = NULL;
    }
	
    shape = NULL;
	motionState = NULL;
	tr.setIdentity();
	mass = 0.0f;
	inertia.setZero();
    callback = NULL;
}

void GameObject::updateTransform() {
    if (!rootNode) return;

	Ogre::Vector3 pos = rootNode->getPosition();
	tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
	Ogre::Quaternion qt = rootNode->getOrientation();
	tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
    
    if(motionState){
		motionState->updateTransform(tr);
	}
}

void GameObject::addToSimulator() {
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	updateTransform();
	motionState = new OgreMotionState(tr, rootNode);
	//motionState = new btDefaultMotionState(tr);

    if (simulator) {
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        if (mass != 0.0f) 
            shape->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
        rbInfo.m_restitution = this->restitution;
        rbInfo.m_friction = this->friction;
        body = new btRigidBody(rbInfo);
        body->setUserPointer(this);

        context = new CollisionContext();
        callback = new ContactSensorCallback(*body, *context);
        
        simulator->addObject(this);
    }
}

btRigidBody* GameObject::getBody(){
	return this->body;
}

void GameObject::setKinematic(){
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);
}

void GameObject::move(Ogre::Real x, Ogre::Real y, Ogre::Real z){
    if (!rootNode) return;
	//body->setLinearVelocity(btVector3(x, y, z));
	
	//translate based on current node's local axes
	rootNode->translate(rootNode->getLocalAxes(), x, y, z);
	
	//translates based on parent axes
	//rootNode->setPosition(rootNode->getPosition() + Ogre::Vector3(x, y, z));
}

void GameObject::rotate(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Node::TransformSpace val_x, Ogre::Node::TransformSpace val_y, Ogre::Node::TransformSpace val_z){
    if (!rootNode) return;
	rootNode->yaw(Ogre::Degree(x), val_x);
	rootNode->pitch(Ogre::Degree(y), val_y);
	//rootNode->roll(Ogre::Degree(z), val_z);
}

/*void GameObject::rotateQuat(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Real w){

}*/

Ogre::SceneNode& GameObject::getNode(){
	return *(this->rootNode);
}

bool GameObject::isPaddle(Ogre::String& name) {
    return name == "clientpaddle" || name == "serverpaddle";
}

bool GameObject::isBall(Ogre::String& name) {
    return name == "clientball" || name == "serverball";
}

bool GameObject::isBox(Ogre::String& name) {
	return name == "ceiling" || name == "WallPosXNode" || name == "WallNegXNode" || name == "WallPosZNode" || name == "WallNegZNode" || name == "floor";
}

