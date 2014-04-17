#pragma once

#include <btBulletDynamicsCommon.h>
#include "BaseApplication.h"
#include "Ball.h"
#include "Box.h"
#include "Simulator.h"
#include "Surface.h"
#include "Wall.h"
#include "Target.h" 
#include "UDPNetEnt.h" 
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <iterator>

#include <SDL_net.h>

class ClientToServer;
class ServerToClient;

#define NUM_PLAYERS 10

class Server {
protected:
    TCPsocket TCPsd;
    IPaddress srvadd;
    int numConnected;
    UDPNetEnt* ents[NUM_PLAYERS - 1];

public:
    Server(int serverPort);
    ~Server();
    void awaitConnections();
    void sendMsg(ServerToClient& data);
    bool recMsg(ClientToServer& data, int index);
};

