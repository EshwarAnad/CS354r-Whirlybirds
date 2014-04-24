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

int Server::awaitConnections(){
    if (numConnected < NUM_PLAYERS - 1) {
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
                char clientAddr[100] = {0};
                sprintf(clientAddr, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
                
                ents[numConnected] = new UDPNetEnt();
                ents[numConnected]->initSending(clientAddr, 49153);
                ents[numConnected]->initReceiving(32100 + numConnected);
                
                numConnected += 1;
                return numConnected;
            }
            else {
                fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
            }
        }
    }

    return -1;
}

void Server::sendMsg(ServerToClient& data) {
    int numBytes = sizeof(MetaData) + sizeof(HeliInfo)*data.meta.numPlaying;

    assert(numBytes < 512 && "UDP packet is 512 bytes... not sure if you can send more than that, broski");
    
    for (int i = 0; i < numConnected; i++) {
        data.meta.clientIndex = i + 1;
        ents[i]->sendMsg(reinterpret_cast<char*>(&data), numBytes);
    }
}

bool Server::recMsg(ClientToServer& data, int index) {
    if (index < numConnected) {
        return ents[index]->recMsg(reinterpret_cast<char*>(&data));
    }
    return false;
}


