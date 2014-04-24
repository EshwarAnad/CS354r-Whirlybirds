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
    std::vector<Ogre::Vector3> positions;

public:
	Heli* helis[NUM_PLAYERS];
    Heli* heli; // our helicopter
    Level* level;
	Ball* powerup;
    Simulator* simulator;
    Ogre::SceneManager* mSceneMgr;
    
    Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient, bool isSinglePlayer);
    ~Game();

    void printNodes(Ogre::SceneNode::ChildNodeIterator it, Ogre::String indent);
    void setDataFromServer(ServerToClient& data);
    ServerToClient& getServerToClientData(void);
    void setDataFromClient(ClientToServer& data, int i);
    ClientToServer& getClientToServerData(void);
    void rotateHeliProps(Ogre::Real t);
	void spawnPowerup(void);
    void makeNewHeli(int index);

    void display(void); // debugging
};

Game::Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient, bool isSinglePlayer)
: simulator(simulator), mSceneMgr(mSceneMgr)
{
	srand(time(NULL));

	sim = simulator;
	mgr = mSceneMgr;
    static Ogre::Real WORLDSCALE = 3.0;
    Ogre::Vector3 origin(0, 0, 0);
   
    // level 
    level = new Level("mylevel", mgr, sim, WORLDSCALE, origin, 0.9, 0.1, "");
	
    // powerup
	spawnPowerup();

    //iterate through all childs of root (debugging purposes)
    Ogre::SceneNode* theRoot = mSceneMgr->getRootSceneNode();
    Ogre::SceneNode::ChildNodeIterator rootIt = theRoot->getChildIterator();
    printNodes(rootIt, "");

    // different spawn positions
    positions.push_back(Ogre::Vector3(300,300,300));  
    positions.push_back(Ogre::Vector3(-300,300,-300));  
    positions.push_back(Ogre::Vector3(-59.59964, 512.620789, -469.511108));
    positions.push_back(Ogre::Vector3(53.735020, 535.609009, -21.95671));

    // helicopter(s)
    for (int i = 0; i < NUM_PLAYERS; i++) {
        helis[i] = NULL;
    }

    if (!isClient || isSinglePlayer) { 
        makeNewHeli(0);
        heli = helis[0];
        heli->addToSimulator();
        heli->setKinematic();
        level->addToSimulator();
        powerup->addToSimulator();
    }
}

void Game::makeNewHeli(int index) {
    assert(index >= 0 || index < NUM_PLAYERS);
            
    char name[100];
    sprintf(name, "heli%d", index);
    
    int pos = rand()%positions.size();

    printf("added heli #%d at %d\n", index, pos);

    helis[index] = new Heli(name, mSceneMgr, simulator, 3.0, 1.0, positions[pos], 0.9, 0.1, "Game/Helicopter");
}

void Game::rotateHeliProps(Ogre::Real t) {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (helis[i]) {
            helis[i]->animate(t);
        }
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
        if (helis[i]) {
           delete helis[i];
        }
    }
}

void Game::setDataFromClient(ClientToServer& data, int i) {
    assert(helis[i] != NULL && "we don't have a heli for this client!");

    helis[i]->getNode().setPosition(data.pose.pos);
    helis[i]->getNode().setOrientation(data.pose.orient);
}

ClientToServer& Game::getClientToServerData(void) {
    cdata_out.pose.pos = heli->getNode().getPosition();
    cdata_out.pose.orient = heli->getNode().getOrientation();
    
    return cdata_out;
}

void Game::setDataFromServer(ServerToClient& data) {
    for (int i = 0; i < data.meta.numPlaying; i++) {
        assert(data.heliPoses[i].exists && "server gave us a heli that doesn't exist!");

        int index = data.heliPoses[i].index;

        if (helis[index] == NULL) {
            makeNewHeli(index);
        } else if (helis[index] != NULL && index == data.meta.clientIndex) {
            // TODO: this makes it so we can move our heli, but it also
            //  makes it so the server can't tell us how our heli should 
            //  behave (so we can go thru walls)
            continue; 
        }

        helis[index]->getNode().setPosition(data.heliPoses[i].pos);
        helis[index]->getNode().setOrientation(data.heliPoses[i].orient);
    }
    
    heli = helis[data.meta.clientIndex];
    
    assert(data.meta.clientIndex != 0 && "our heli is being set to the server's!");
    assert(heli != NULL && "we haven't received any data for our heli!");
}

ServerToClient& Game::getServerToClientData(void) {
    sdata_out.meta.sound = 0;
    sdata_out.meta.clientIndex = 0;
    sdata_out.meta.numPlaying = 0;
   
    int np = 0;
 
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (helis[i]) {
            sdata_out.heliPoses[np].pos = helis[i]->getNode().getPosition();
            sdata_out.heliPoses[np].orient = helis[i]->getNode().getOrientation();
            sdata_out.heliPoses[np].index = i;
            sdata_out.heliPoses[np].exists = true;
            np += 1;
        } 
    }

    sdata_out.meta.numPlaying = np;
    
    for (int j = np; j < NUM_PLAYERS; j++) {
        sdata_out.heliPoses[j].exists = false;
    }

    return sdata_out;
}

void Game::spawnPowerup(void) {
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

void Game::display(void) {
    printf("Game state (%d players):\n", NUM_PLAYERS);

    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("  heli #%d\n", i);

        if (helis[i] == NULL) {
            printf("    (NULL)\n");
            continue;
        }
        else if (heli == helis[i]) {
            printf("    (this is our heli)\n");
        }

        printf("    loc: %f %f %f\n", 
            helis[i]->getNode().getPosition().x,
            helis[i]->getNode().getPosition().y,
            helis[i]->getNode().getPosition().z);
    }  
}
