#pragma once

#include "Server.h"

class Game {
protected:
    ServerToClient data_out;

public:
	Heli* helis[Server::NUM_PLAYERS];
    Heli* heli; // our helicopter
    Box* box;
    
    Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient);
    ~Game();
    void setDataFromServer(ServerToClient& data);
    ServerToClient getServerToClientData(void);
};

Game::Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient)
{
    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/BeachStones");
    
    helis[0] = new Heli("heli0", mSceneMgr, simulator, 3.0, 1.0, Ogre::Vector3(0.0, 0.0, 45.0), 0.9, 0.1, "Game/Helicopter");
	
    for (int i = 1; i < Server::NUM_PLAYERS; i++) {
        char name[100];
        sprintf(name, "heli%d", i);
        helis[i] = new Heli(name, mSceneMgr, simulator, 3.0, 1.0, Ogre::Vector3(0.0, 0.0, 45.0), 0.9, 0.1, "Game/Helicopter");
    }

    if (!isClient) {
        heli = helis[0];
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
    }
    
    heli = helis[data.clientIndex];
}

ServerToClient Game::getServerToClientData(void) {
    data_out.sound = 0;
    data_out.clientIndex = 0;
    
    for (int i = 0; i < Server::NUM_PLAYERS; i++) {
        data_out.heliPoses[i].pos = helis[i]->getNode().getPosition();
        data_out.heliPoses[i].orient = helis[i]->getNode().getOrientation();
    }
    
    return data_out;
}

