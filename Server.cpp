#include "Server.h"
#include "ServerToClient.h"
#include "ClientToServer.h"

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

void Server::sendMsg(ServerToClient& data) {
    for (int i = 0; i < numConnected; i++) {
        data.clientIndex = i;
        ents[i]->sendMsg(reinterpret_cast<char*>(&data), sizeof(ServerToClient));
    }
}

bool Server::recMsg(ClientToServer& data, int index) {
    if (index < numConnected) {
        return ents[index]->recMsg(reinterpret_cast<char*>(&data));
    }
    return false;
}


