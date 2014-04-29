#pragma once

#include "ServerToClient.h"

struct ClientToServer {
    bool disconnecting;
    float xMove, yMove, zMove, mMove;

    ClientToServer() {
        xMove = yMove = zMove = mMove = 0;
        disconnecting = false;
    }
};

