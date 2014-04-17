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
    bool hasInitSending, hasInitReceiving;
    
    UDPNetEnt();
    ~UDPNetEnt();

    void initSending(char* sendAddress, int sendPort);
    void initReceiving(int recPort);
    
    bool recMsg(char* out_data); // returns success
    void sendMsg(char* data, int len);
};

