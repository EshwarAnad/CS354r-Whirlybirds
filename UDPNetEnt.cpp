#include "UDPNetEnt.h"

UDPNetEnt::UDPNetEnt() :
hasInitSending(false), hasInitReceiving(false){
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

void UDPNetEnt::initSending(char* sendAddress, int sendPort) {
    hasInitSending = true;

    /* Open a socket on random port */
    if (!(sendSock = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    /* Resolve server name  */
    if (SDLNet_ResolveHost(&sendIP, sendAddress, sendPort) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", 
            sendAddress, 
            sendPort, 
            SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    /* Allocate memory for the packet */
    if (!(sendPack = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

void UDPNetEnt::initReceiving(int recPort) {
    hasInitReceiving = true;

    /* Open a socket */
    if (!(recSock = SDLNet_UDP_Open(recPort)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    /* Make space for the packet */
    if (!(recPack = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

// returns success
bool UDPNetEnt::recMsg(char* data) {
    if (SDLNet_UDP_Recv(recSock, recPack))
    {
        memcpy(data, (char*)recPack->data, recPack->len);
        return true;
    } else {
        return false;
    }
}

void UDPNetEnt::sendMsg(char *data, int len) {
    /* Set the destination host */
    sendPack->address.host = sendIP.host;  
    /* And destination port */
    sendPack->address.port = sendIP.port;  

    sendPack->len = len;
    memcpy(sendPack->data, data, len);

    /* This sets the p->channel */
    SDLNet_UDP_Send(sendSock, -1, sendPack); 
}

UDPNetEnt::~UDPNetEnt() {
    SDLNet_FreePacket(recPack);
    SDLNet_FreePacket(sendPack);
    SDLNet_Quit();
}

