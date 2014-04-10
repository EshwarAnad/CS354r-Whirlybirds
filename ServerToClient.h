#pragma once

#include <btBulletDynamicsCommon.h>
#include "Server.h"

struct HeliPose {
    Ogre::Vector3 pos;
    Ogre::Quaternion orient;
};

class ServerToClient {
	public:
        HeliPose heliPoses[Server::NUM_PLAYERS];
        int clientIndex; // set by the server, specific to the client it sends to
  		int sound;
};

