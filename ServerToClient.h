#pragma once

#include <btBulletDynamicsCommon.h>
#include "Server.h"

struct RocketInfo {
    Ogre::Vector3 pos;
    Ogre::Quaternion orient;
    bool exists;
};

struct HeliInfo {
    Ogre::Vector3 pos;
    Ogre::Quaternion orient;
    bool exists;
    int index;
    unsigned char deaths;

    HeliInfo() {
        exists = false;
        index = 0;
        deaths = 0;
    }
};

struct MetaData {
    bool shutdown;
    int clientIndex; // set by the server, specific to the client it sends to
    int sound;
    int numPlaying;
    long time;
    int numRockets;
    RocketInfo rockets[MAX_ROCKETS];
};

struct ServerToClient {
    MetaData meta;
    HeliInfo heliPoses[NUM_PLAYERS];

    ServerToClient() {
        meta.sound = 0;
        meta.numPlaying = 0;
        meta.numRockets = 0;
        meta.clientIndex = -1;
        meta.shutdown = false;
    }
};

