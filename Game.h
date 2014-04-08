#include "Server.h"

class Game {
public:
	Heli* helis[Server::NUM_PLAYERS];
    Box* box;
    
    int index;
    bool isClient;

    Game();
    void setDataFromServer(ServerToClienti& data);
    ServerToClient* getServerToClientData(void);
}

Game::Game(Simulator* simulator, mSceneMgr) 
{
    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/BeachStones");
	
    for (int i = 0; i < Server::NUM_PLAYERS; i++) {
        char name[100];
        sprintf(name, "heli%d", i);
        helis[i] = new Heli(name, mSceneMgr, simulator, 3.0, 1.0, Ogre::Vector3(1000.0, 1000.0, 1000.0), 0.9, 0.1, "Game/Helicopter");
    }
}

Game::~Game() {
    delete box;
    
    for (int i = 0; i < Server::NUM_PLAYERS; i++) {
        delete helis[i];
    }
}

void Game::setDataFromServer(ServerToClient& data) {
    for (int i = 0; i < Server::NUM_PLAYERS; i++) {
        data.heliTrans[i];
    }
}

ServerToClient* Game::getServerToClientData(void) {
    ServerToClient* data = new ServerToClient();
    
    for (int i = 0; i < Server::NUM_PLAYERS; i++) {
        data->heliTrans[i] = helis[i]->getNode()->getTrans();
    }
    
    return data;
}

