#include "Server.h"

class Game {
public:
	Heli* helis[Server::NUM_PLAYERS];
    Heli* ourHeli;
    Box* box;
    
    Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr);
    void setDataFromServer(ServerToClienti& data);
    void getServerToClientData(ServerToClient* data_out);
}

Game::Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr)
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
        helis[i]->getNode().setPosition(data.heliPoses[i].pos);
        helis[i]->getNode().setOrientation(data.heliPoses[i].orient);
        data.heliTrans[i];
    }
    
    ourHeli = helis[data.clientIndex];
}

void Game::getServerToClientData(ServerToClient* data_out) {
    for (int i = 0; i < Server::NUM_PLAYERS; i++) {
        data_out->heliPoses[i].pos = helis[i]->getNode().getPosition();
        data_out->heliPoses[i].orient = helis[i]->getNode().getOrientation()
    }
}

