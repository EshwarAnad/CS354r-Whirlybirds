#pragma once

#include <btBulletDynamicsCommon.h>
#include "Server.h"

struct HeliPose {
    Ogre::Vector3 pos;
    Ogre::Quaternion orient;
    bool exists;
    int index;
};

struct ServerToClient {
    int clientIndex; // set by the server, specific to the client it sends to
    int sound;
    int numPlaying;
    HeliPose heliPoses[NUM_PLAYERS];
};

