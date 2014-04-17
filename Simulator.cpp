#include <iostream>
#include "Simulator.h"
#include "ContactSensorCallback.h"

Simulator::Simulator(){
	// Collision configuration contains default setup for memory, collision setup.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	// Use the default collision dispatcher
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase. Can also try btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	//the default constraint solver. For parallel processing you can use different solver.
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	gravity = -9.8f;
    dynamicsWorld->setGravity(btVector3(0.0f, gravity, 0.0));

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

    // Initialize the sound system
    soundSystem = new SoundSystem();
    soundOn = true;
}

Simulator::~Simulator() {

}

void Simulator::setGravity(float g) {
	if (g > 100.f || g < -100.f) return;

    gravity = g;
    dynamicsWorld->setGravity(btVector3(0.0f, gravity, 0.0f));
}

void Simulator::addObject (GameObject* o) {
	objList.push_back(o);
	
    //use default collision group/mask values (dynamic/kinematic/static)
	dynamicsWorld->addRigidBody(o->getBody());
    std::cout << "ridig body: " << o->getBody() << std::endl;
}

bool Simulator::removeObject(GameObject* o) {
	dynamicsWorld->removeRigidBody(o->getBody());
	for (int i = 0; i < objList.size(); i++) {
		if (objList[i]->name == o->name)
			objList.erase(objList.begin()+i);
	}
}

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep){
	//do we need to update positions in simulator for dynamic objects?
	dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);

    for (int i = 0; i < objList.size(); i++) {
        objList[i]->callback->ctxt.hit = false;

        for (int j = 0; j < objList.size(); j++) {
            if (i == j) { continue; }

            dynamicsWorld->contactPairTest(
                objList[i]->getBody(), 
                objList[j]->getBody(), 
                *(objList[i]->callback)
                );
        }
    }

    for (int i = 0; i < objList.size(); i++) {
        objList[i]->update();
    }
}
