#pragma once

#include <Ogre.h>
#include "Server.h"
#include "Simulator.h"

class Game {
protected:
    ServerToClient sdata_out;
    ClientToServer cdata_out;

public:
	Heli* helis[NUM_PLAYERS];
    Heli* heli; // our helicopter
    Box* box;
    
    Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient);
    ~Game();
    void setDataFromServer(ServerToClient& data);
    ServerToClient& getServerToClientData(void);
    void setDataFromClient(ClientToServer& data, int i);
    ClientToServer& getClientToServerData(void);
};

Game::Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient)
{
    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/BeachStones");
    
    helis[0] = new Heli("heli0", mSceneMgr, simulator, 3.0, 1.0, Ogre::Vector3(0.0, 0.0, 45.0), 0.9, 0.1, "Game/Helicopter");
	
    for (int i = 1; i < NUM_PLAYERS; i++) {
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
    
    for (int i = 0; i < NUM_PLAYERS; i++) {
        delete helis[i];
    }
}

void Game::setDataFromClient(ClientToServer& data, int i) {
    helis[i]->getNode().setPosition(data.pose.pos);
    helis[i]->getNode().setOrientation(data.pose.orient);
}

ClientToServer& Game::getClientToServerData(void) {
    cdata_out.pose.pos = heli->getNode().getPosition();
    cdata_out.pose.orient = heli->getNode().getOrientation();
    return cdata_out;
}

void Game::setDataFromServer(ServerToClient& data) {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (heli != NULL && i == data.clientIndex) continue;
        helis[i]->getNode().setPosition(data.heliPoses[i].pos);
        helis[i]->getNode().setOrientation(data.heliPoses[i].orient);
    }
    
    heli = helis[data.clientIndex];
}

ServerToClient& Game::getServerToClientData(void) {
    sdata_out.sound = 0;
    sdata_out.clientIndex = 0;
    
    for (int i = 0; i < NUM_PLAYERS; i++) {
        sdata_out.heliPoses[i].pos = helis[i]->getNode().getPosition();
        sdata_out.heliPoses[i].orient = helis[i]->getNode().getOrientation();
    }
    
    return sdata_out;
}

