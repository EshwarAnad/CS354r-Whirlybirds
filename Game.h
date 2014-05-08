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
    bool isClient;
    bool isSinglePlayer;
    unsigned int rocketNum;

public:
	Heli* helis[NUM_PLAYERS];
    Heli* heli; // our helicopter
    Level* level;
	Ball* powerup;
    std::vector<Rocket*> rockets;
    Rocket* rocket;
    //Ogre::SceneManager* mgr;
    Simulator* simulator;
    Ogre::SceneManager* mSceneMgr;
    
    Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient, bool isSinglePlayer);
    ~Game();

    void printNodes(Ogre::SceneNode::ChildNodeIterator it, Ogre::String indent);
    void setDataFromServer(ServerToClient& data);
    ServerToClient& getServerToClientData(void);
    void setDataFromClient(ClientToServer& data, int i);
    void rotateHeliProps(Ogre::Real t);
	void spawnPowerup(void);
    void makeNewHeli(int index);
    void addRocket(Heli* heli);
    Ogre::Vector3 getSpawnPos();

    void display(void); // debugging
};

Game::Game(Simulator* simulator, Ogre::SceneManager* mSceneMgr, bool isClient, bool isSinglePlayer)
: simulator(simulator), mSceneMgr(mSceneMgr), isClient(isClient), isSinglePlayer(isSinglePlayer)
{
	srand(time(NULL));

	sim = simulator;
	mgr = mSceneMgr;
    static Ogre::Real WORLDSCALE = 3.0;
    Ogre::Vector3 origin(0, 0, 0);
    rocketNum = 0;
   
    // level 
    level = new Level("mylevel", mgr, sim, WORLDSCALE, origin, 0.9, 0.1, "Examples/Rockwall");
	
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
        level->addToSimulator();
        makeNewHeli(0);
        heli = helis[0];
    }
}

void Game::addRocket(Heli* mheli) {
    Ogre::Vector3 pos;
    Ogre::Matrix3 ax;
    
    if (mheli != NULL) {
        pos = mheli->getNode().getPosition();
        pos = pos + Ogre::Vector3(0, 20, 0);
        ax = mheli->getNode().getLocalAxes();
    }
    
    char name[100] = {0};
    sprintf(name, "rocket%d", rocketNum);
    rocketNum++;
    rockets.push_back(new Rocket(name, mSceneMgr, simulator, 3.0, 1.0, pos, ax, 5.0, "Game/Rocket"));

    if (mheli != NULL) { 
        Ogre::Quaternion angle = mheli->getNode().getOrientation();
        Ogre::Vector3 pTemp(angle* Ogre::Vector3::NEGATIVE_UNIT_Z * 700);
        rockets[rockets.size()-1]->addToSimulator();
        rockets[rockets.size()-1]->getBody()->setLinearVelocity(btVector3(pTemp.x, pTemp.y, pTemp.z));
        //rockets[game->rockets.size()-1]->getBody()->setLinearVelocity(btVector3(0, -10, -500));
    }
}

void Game::makeNewHeli(int index) {
    assert(index >= 0 || index < NUM_PLAYERS);
            
    char name[100];
    sprintf(name, "heli%d", index);
    
    int pos = rand()%positions.size();

    helis[index] = new Heli(name, mSceneMgr, simulator, 3.0, 1.0, positions[pos], 0.9, 0.1, "Game/Helicopter");
    
    if (isSinglePlayer || !isClient) {
        helis[index]->addToSimulator();
        helis[index]->setKinematic();
    }

    char lightName[100];
    sprintf(lightName, "heli%dlight", index);

    Ogre::Light* spotLight = mSceneMgr->createLight(lightName);
    spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
   
    spotLight->setDiffuseColour(0.5, 0.5, 0.5);
    spotLight->setSpecularColour(0.5, 0.5, .5); 
    spotLight->setDirection(0,0,-1);
    spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));	
   
    char nodeName[100];
    sprintf(nodeName, "heli%dlightnode", index);
 
    (&(helis[index]->getNode()))->createChildSceneNode(nodeName);
	mSceneMgr->getSceneNode(nodeName)->attachObject(spotLight);
	mSceneMgr->getSceneNode(nodeName)->translate(0.0, 0.0, 0.0);
    
    
    printf("added %s at %d\n", name, index, pos);
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
    if (helis[i] == NULL) {
        // this can happen if a client has disconnected, but we still get a few packets from them
        printf("we don't have a heli for client #%d!", i);
        // assert(helis[i] != NULL && "we don't have a heli for this client!");
        return;
    }

    if (data.disconnecting) {
        printf("removing heli from %d\n", i);
        simulator->removeObject(helis[i]->chass);
        simulator->removeObject(helis[i]->prop);
        delete helis[i];
        helis[i] = NULL;
        printf("removed heli from %d\n", i);
    } else {
        helis[i]->move(data.xMove, data.yMove, data.zMove);
        helis[i]->rotate(-data.mMove*0.035);
        helis[i]->updateTransform();
        
        if (data.firingRocket) {
            addRocket(helis[i]);
        }
    }
}

void Game::setDataFromServer(ServerToClient& data) {
    bool updatedHelis[NUM_PLAYERS] = {0};

    for (int i = 0; i < data.meta.numPlaying; i++) {
        assert(data.heliPoses[i].exists && "server gave us a heli that doesn't exist!");

        int index = data.heliPoses[i].index;

        if (helis[index] == NULL) {
            makeNewHeli(index);
        } 
        
        helis[index]->getNode().setPosition(data.heliPoses[i].pos);
        helis[index]->getNode().setOrientation(data.heliPoses[i].orient);
        updatedHelis[index] = true;
    }
   
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (!updatedHelis[i] && helis[i] != NULL) {
            printf("removing heli from %d\n", i);
            delete helis[i];
            helis[i] = NULL;
            printf("removed heli from %d\n", i);
        }
    }
 
    heli = helis[data.meta.clientIndex];
   
    assert(data.meta.clientIndex != 0 && "our heli is being set to the server's!");
    assert(heli != NULL && "we haven't received any data for our heli!");
    
    for (int i = 0; i < data.meta.numRockets && i < MAX_ROCKETS; i++) {
        if (i == rockets.size()) {
            printf("  adding new rocket\n");
            addRocket(NULL);
        }
        
        assert(data.meta.rockets[i].exists);       
        
        rockets[i]->getNode().setPosition(data.meta.rockets[i].pos);
        rockets[i]->getNode().setOrientation(data.meta.rockets[i].orient);
    }
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

    int j = 0;
    for (int i = rockets.size() - 1; i >= 0; i--) {
        if (j == MAX_ROCKETS) {
            break;
        }

        printf("sending rocket %d of %d\n", i, rockets.size());

        sdata_out.meta.rockets[j].pos = rockets[i]->getNode().getPosition();
        sdata_out.meta.rockets[j].orient = rockets[i]->getNode().getOrientation();
        sdata_out.meta.rockets[j].exists = true;
        
        j++;
    }

    sdata_out.meta.numRockets = rockets.size();
    if (sdata_out.meta.numRockets > MAX_ROCKETS) {
        sdata_out.meta.numRockets = MAX_ROCKETS;
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

    if (!isClient || isSinglePlayer) {
		powerup = new Ball(nym, mgr, sim, 20.0, 1.0, Ogre::Vector3(x, 300.0, z), 1.0, 1.0, tex);
		powerup->addToSimulator();
	}
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

Ogre::Vector3 Game::getSpawnPos(){
    int pos = rand()%positions.size();
    return positions[pos];
}
