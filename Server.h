#ifndef __Server_h_
#define __Server_h_

#include <btBulletDynamicsCommon.h>
#include "BaseApplication.h"
#include "Ball.h"
#include "Box.h"
#include "Simulator.h"
#include "Surface.h"
#include "Wall.h"
#include "Target.h" 
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <iterator>

#include <SDL_net.h>

const int _NUM_PLAYERS = 3;

class Server {
protected:
    TCPsocket TCPsd;
    IPaddress srvadd;
    int numConnected;
    UDPNetEnt* ents[_NUM_PLAYERS];

public:
    static const int NUM_PLAYERS;
    Server(int serverPort);
    ~Server();
    void awaitConnections();
    void sendMsg(char *data, int len);
    bool recMsg(char* data, int index);
};

const int Server::NUM_PLAYERS = _NUM_PLAYERS;

Server::Server(int serverPort) {
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Resolve server name  */
    if (SDLNet_ResolveHost(&srvadd, NULL, serverPort) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(NULL %d): %s\n", 
            serverPort, 
            SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if(!(TCPsd = SDLNet_TCP_Open(&srvadd)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    numConnected = 0;
}

Server::~Server() {
    for (int i = 0; i < numConnected; i++) {
        delete ents[i];
    }
    SDLNet_Quit();
}

void Server::awaitConnections(){
    if (numConnected < NUM_PLAYERS) {
        if (TCPsocket TCPcsd = SDLNet_TCP_Accept(TCPsd)) {
            /*can now communicate with client using csd socket*/
            printf("Connection success\n");
            
            /* Get the remote address */
            if (IPaddress* remoteIP = SDLNet_TCP_GetPeerAddress(TCPcsd)) {
                /* Print the address, converting in the host format */
                printf("Host connected: %x %d\n", 
                    SDLNet_Read32(&remoteIP->host), 
                    SDLNet_Read16(&remoteIP->port));
                
                /* Resolve server name  */
                /* hacky way to get IPaddress */
                Uint8* addr = (Uint8*) &remoteIP->host;
                char clientAddr[100];
                sprintf(clientAddr, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
                ents[numConnected] = new UDPNetEnt(clientAddr, 49153, 32100);
                
                numConnected += 1;
            }
            else {
                fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
            }
        }
    }
}

void Server::sendMsg(char *data, int len){
    for (int i = 0; i < numConnected; i++) {
        ents[i]->sendMsg(data, len);
    }
}

bool Server::recMsg(char* data, int index){
    if (numConnected > 0) {
        return ents[index]->recMsg(data);
    }
    return false;
}

#endif
