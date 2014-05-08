#pragma once

#include "ServerToClient.h"

struct ClientToServer {
    bool disconnecting;
    float xMove, yMove, zMove, mMove;
    bool firingRocket;
    time_t servertime;

    ClientToServer() {
        xMove = yMove = zMove = mMove = 0;
        disconnecting = false;
        firingRocket = false;
    }
};

