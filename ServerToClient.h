#pragma once

#include <btBulletDynamicsCommon.h>
#include "Server.h"

struct MissileInfo {
    Ogre::Vector3 pos;
    Ogre::Quaternion orient;
    bool exists;
};

struct HeliInfo {
    Ogre::Vector3 pos;
    Ogre::Quaternion orient;
    bool exists;
    int index;

    HeliInfo() {
        for (int i = 0; i < MAX_MISSILES_PER_PLAYER; i++) {
            missiles[i].exists = false;
        }
    }
};

struct MetaData {
    bool shutdown;
    int clientIndex; // set by the server, specific to the client it sends to
    int sound;
    int numPlaying;
};

struct ServerToClient {
    MetaData meta;
    MissileInfo missiles[MAX_MISSILES];
    HeliInfo heliPoses[NUM_PLAYERS];

    ServerToClient() {
        meta.sound = 0;
        meta.numPlaying = 0;
        meta.clientIndex = -1;
        meta.shutdown = false;

        for (int i = 0; i < NUM_PLAYERS; i++) {
            heliPoses[i].exists = false;
        }
    }
};

