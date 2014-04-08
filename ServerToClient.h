#ifndef __ServertoClient_h_
#define __ServertoClient_h_

#include <btBulletDynamicsCommon.h>
#include "Server.h"

class ServerToClient {
	public:
		btTransform heliTrans[Server::NUM_PLAYERS];
        int numConnected;
        int clientIndex;
  		int sound;
		ServerToClient();
};

ServerToClient::ServerToClient() {}

#endif
