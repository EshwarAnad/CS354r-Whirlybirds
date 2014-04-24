#pragma once

#include <btBulletDynamicsCommon.h>
#include "Server.h"

struct HeliInfo {
    Ogre::Vector3 pos;
    Ogre::Quaternion orient;
    bool exists;
    int index;
};

struct MetaData {
    int clientIndex; // set by the server, specific to the client it sends to
    int sound;
    int numPlaying;
};

struct ServerToClient {
    MetaData meta;
    HeliInfo heliPoses[NUM_PLAYERS];
};

