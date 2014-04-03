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

class Server {
protected:
    TCPsocket TCPsd, TCPcsd;
    IPaddress srvadd, *remoteIP;
    bool connect;

public:
    UDPNetEnt* ent;
    Server(int serverPort);
    ~Server();
    void awaitConnections();
    void sendMsg(char *data, int len);
    bool recMsg(char* data);
};

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
    connect = false;
}

Server::~Server() {
    SDLNet_Quit();
    delete ent;
}

void Server::awaitConnections(){
    if(!connect){
        if(TCPcsd = SDLNet_TCP_Accept(TCPsd)){
            printf("Connection success\n");
            /*can now communicate with client using csd socket*/

            /* Get the remote address */
            if(remoteIP = SDLNet_TCP_GetPeerAddress(TCPcsd)){
                /* Print the address, converting in the host format */
                printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
                connect = true;
                /* Resolve server name  */
                /* hacky way to get IPaddress */
                Uint8* addr = (Uint8*) &remoteIP->host;
                char clientAddr[100];
                sprintf(clientAddr, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
                ent = new UDPNetEnt(clientAddr, 49153, 32100);
            }
            else
                fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
        }
    }
}

void Server::sendMsg(char *data, int len){
    if(connect)
        ent->sendMsg(data, len);
}

bool Server::recMsg(char* data){
    if(connect)
        return ent->recMsg(data);
    return false;
}

#endif
