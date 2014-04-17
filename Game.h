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
    Level* level;
	Ball* powerup;
    
    Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient);
    ~Game();

    void printNodes(Ogre::SceneNode::ChildNodeIterator it, Ogre::String indent);
    void setDataFromServer(ServerToClient& data);
    ServerToClient& getServerToClientData(void);
    void setDataFromClient(ClientToServer& data, int i);
    ClientToServer& getClientToServerData(void);
};

Game::Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient)
{
    static Ogre::Real WORLDSCALE = 3.0;
    Ogre::Vector3 origin(0, 0, 0);
   
    // level 
    level = new Level("mylevel", mSceneMgr, simulator, WORLDSCALE, origin, 0.9, 0.1, "");
	
    // helicoper(s)
    for (int i = 0; i < NUM_PLAYERS; i++) {
        char name[100];
        sprintf(name, "heli%d", i);
        helis[i] = new Heli(name, mSceneMgr, simulator, 3.0, 1.0, Ogre::Vector3(0.0, 0.0, 45.0), 0.9, 0.1, "Game/Helicopter");
    }

    // powerup
	powerup = new Ball("speed", mSceneMgr, simulator, 20.0, 1.0, Ogre::Vector3(0.0, 300.0, 300.0), 1.0, 1.0, "Game/P1ball");

    //iterate through all childs of root (debugging purposes)
    Ogre::SceneNode* theRoot = mSceneMgr->getRootSceneNode();
    Ogre::SceneNode::ChildNodeIterator rootIt = theRoot->getChildIterator();
    printNodes(rootIt, "");
   
    if (!isClient) { 
        heli = helis[0];
        heli->addToSimulator();
        heli->setKinematic();
        level->addToSimulator();
        powerup->addToSimulator();
    }
}

void Game::printNodes(Ogre::SceneNode::ChildNodeIterator it, Ogre::String indent){
    Ogre::SceneNode* cur;
    while(it.hasMoreElements()){
        cur = dynamic_cast<Ogre::SceneNode*>(it.getNext());
        std::cout << indent << cur->getName() << std::endl;
        printNodes(cur->getChildIterator(), indent + "\t");
    }

}

Game::~Game() {
    delete powerup;
    delete level;   
 
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

