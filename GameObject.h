
#ifndef __GameObject_h_
#define __GameObject_h_

#include <btBulletDynamicsCommon.h>
#include "OgreMotionState.h"
#include <Ogre.h>
#include "ContactSensorCallback.h"

#define DEBUG true


class Simulator;

class GameObject {
	protected:
		Ogre::SceneManager* sceneMgr;
		Simulator* simulator;
		Ogre::SceneNode* rootNode;
		Ogre::Entity* geom;
		btScalar mass;
		btTransform tr;
		btVector3 inertia;
		Ogre::Real restitution;
    	Ogre::Real friction;
	public:
        ContactSensorCallback* callback;
		OgreMotionState* motionState;
		Ogre::String name;
        btRigidBody* body;
		btCollisionShape* shape;
		
        GameObject(Ogre::String, Ogre::SceneManager*, Simulator*, Ogre::Real, Ogre::Real);
		virtual void updateTransform();
		void addToSimulator();
		btRigidBody* getBody();
		void move(Ogre::Real, Ogre::Real, Ogre::Real);
		void rotate(Ogre::Real, Ogre::Real, Ogre::Real, Ogre::Node::TransformSpace val_x = Ogre::Node::TS_LOCAL, Ogre::Node::TransformSpace val_y = Ogre::Node::TS_LOCAL, Ogre::Node::TransformSpace val_z = Ogre::Node::TS_LOCAL);
		Ogre::SceneNode& getNode();
		void setKinematic();

        virtual void update() {}

    protected:
        bool isPaddle(Ogre::String& name);
        bool isBall(Ogre::String& name);
        bool isBox(Ogre::String& name);
}; 

#endif
