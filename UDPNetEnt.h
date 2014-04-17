#pragma once

#include <SDL_net.h>

class UDPNetEnt {
protected:
    UDPsocket recSock;
    UDPpacket *recPack;
    
    UDPsocket sendSock;
    IPaddress sendIP;
    UDPpacket *sendPack;

public:
    UDPNetEnt(char* sendAddress, int sendPort, int recPort);
    ~UDPNetEnt();
    
    bool recMsg(char* out_data); // returns success
    void sendMsg(char* data, int len);
};

