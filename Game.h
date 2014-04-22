#pragma once

#include <Ogre.h>
#include "Server.h"
#include "Simulator.h"

class Game {
protected:
    ServerToClient sdata_out;
    ClientToServer cdata_out;
	Simulator* sim;
	Ogre::SceneManager* mgr;

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
	void spawnPowerup(void);
};

Game::Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient)
{
	sim = simulator;
	mgr = mSceneMgr;
    static Ogre::Real WORLDSCALE = 3.0;
    Ogre::Vector3 origin(0, 0, 0);
   
    // level 
    level = new Level("mylevel", mgr, sim, WORLDSCALE, origin, 0.9, 0.1, "");
	
    // helicoper(s)
    for (int i = 0; i < NUM_PLAYERS; i++) {
        char name[100];
        sprintf(name, "heli%d", i);
        helis[i] = new Heli(name, mgr, sim, 3.0, 1.0, Ogre::Vector3(300.0,300.0, 300.0), 0.9, 0.1, "Game/Helicopter");
    }

    if (!isClient) {
        heli = helis[0];
    }
	
	spawnPowerup();

    //iterate through all childs of root (debugging purposes)
    Ogre::SceneNode* theRoot = mSceneMgr->getRootSceneNode();
    Ogre::SceneNode::ChildNodeIterator rootIt = theRoot->getChildIterator();
    printNodes(rootIt, "");
    
    heli->addToSimulator();
	heli->setKinematic();
    level->addToSimulator();
    powerup->addToSimulator();
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

void Game::spawnPowerup(void) {
	srand (time(NULL));
	int power = rand()%4;
	Ogre::String nym;
	Ogre::String tex;
	Ogre::Vector3 pos;
	Ogre::Real x, z;
	//determine powerup type
	switch(power) {
		case SPEED:
			nym = "speed";
			tex = "Game/speedBall";
			x = 0.0;
			z = 300.0;
			break;
		case POWER:
			nym = "power";
			tex = "Game/powerBall";
			x = 300.0;
			z = 0.0;
			break;
		case HEALTH:
			nym = "health";
			tex = "Game/healthBall";
			x = 0.0;
			z = -330.0;
			break;
		case SHIELD:
			nym = "shield";
			tex = "Game/shieldBall";
			x = -280.0;
			z = 0.0;
			break;
		default:
			nym = "speed";
			tex = "Game/speedBall";
			x = 0.0;
			z = 300.0;
			break;
	}
	powerup = new Ball(nym, mgr, sim, 20.0, 1.0, Ogre::Vector3(x, 300.0, z), 1.0, 1.0, tex);
}
