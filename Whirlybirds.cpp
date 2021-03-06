/*
-----------------------------------------------------------------------------
Filename:    Whirlybirds.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "Whirlybirds.h"

//-------------------------------------------------------------------------------------
Whirlybirds::Whirlybirds()
: simulator(NULL), isClient(false), gameplay(false), isSinglePlayer(false)
{}

//-------------------------------------------------------------------------------------
Whirlybirds::~Whirlybirds(void)
{}

//-------------------------------------------------------------------------------------
void Whirlybirds::createScene(void)
{
    simulator = new Simulator();
	CEGUI::Event::Subscriber* spSub = new CEGUI::Event::Subscriber(&Whirlybirds::singlePlayer, this);
	CEGUI::Event::Subscriber* clientSub = new CEGUI::Event::Subscriber(&Whirlybirds::clientStart, this);
	CEGUI::Event::Subscriber* serverSub = new CEGUI::Event::Subscriber(&Whirlybirds::serverStart, this);
	gui = new GUI(spSub, clientSub, serverSub);
	   
	// Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(10.0f, 10.0f, 10.0f);

	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
}

bool powerupSpawn = false;

bool Whirlybirds::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    static Ogre::Real e_time = 0.0;
	float xMove, yMove, zMove;

    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;
    
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	if (gameplay) {
		//check if powerup needs to spawn
		if (mSceneMgr->hasSceneNode("speed") || mSceneMgr->hasSceneNode("power") || mSceneMgr->hasSceneNode("health") || mSceneMgr->hasSceneNode("shield")) {
			time(&powerupTime);
			powerupSpawn = true;
		}

		if (powerupSpawn) {
			time(&currentTime);
			if (difftime(currentTime, powerupTime) >= 20) {
				game->spawnPowerup();
				powerupSpawn = false;
			}
		}

        //check if helicopter is in bounds
        game->heli->inBounds(game->level->getBounds(), evt.timeSinceLastFrame, gui);
        if(!game->heli->alive)
            game->heli->respawn(game->getSpawnPos(), evt.timeSinceLastFrame, gui);
		xMove = 0.0,
		yMove = 0.0,
		zMove = 0.0;

        if(e_time > 0.0 && e_time < 0.5)
            e_time += evt.timeSinceLastFrame;
        else
            e_time = 0.0;
        

        if(mKeyboard->isKeyDown(OIS::KC_W))
			zMove = -evt.timeSinceLastFrame;
        if (mKeyboard->isKeyDown(OIS::KC_S))
			zMove = evt.timeSinceLastFrame;
        if (mKeyboard->isKeyDown(OIS::KC_A))
			xMove = -evt.timeSinceLastFrame;
        if (mKeyboard->isKeyDown(OIS::KC_D))
			xMove = evt.timeSinceLastFrame;
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			yMove = evt.timeSinceLastFrame;
		if (mKeyboard->isKeyDown(OIS::KC_SPACE))
            yMove = -evt.timeSinceLastFrame;
        if (mMouse->getMouseState().buttonDown(OIS::MB_Left) && e_time == 0.0)
        {
            if (gameplay && (!isClient || isSinglePlayer)) {
                game->addRocket(game->heli);
                simulator->soundSystem->playShootRocket();
            } else {
                clientFiringRocket = true;
            }
            e_time += evt.timeSinceLastFrame;
        }
        /*if (mKeyboard->isKeyDown(OIS::KC_E) && e_time == 0.0){


            if (gameplay && (!isClient || isSinglePlayer)) {
                game->addRocket(game->heli);
                simulator->soundSystem->playShootRocket();
            } else {
                clientFiringRocket = true;
            }
            e_time += evt.timeSinceLastFrame;
        }   */

        if (!isClient || isSinglePlayer) {
            for (int i = 0; i < game->rockets.size(); i++)
            {
                //game->rockets[i]->move();
                game->rockets[i]->updateTransform(evt.timeSinceLastFrame);
                game->rockets[i]->timeToExpire(evt.timeSinceLastFrame);
                if(game->rockets[i]->destroy){
                    //std::cout << "Destroying rocket " << game->rockets[i]->name << " @ " << i << "..." << std::endl;
                    game->rockets[i]->Rocket::~Rocket();
                    //std::cout << "Destroyed." << std::endl;
                    game->rockets.erase(game->rockets.begin()+i);
                    //std::cout << "Removed from vector." << std::endl;
                    
                }
            }
        }
            
        Ogre::Real mMove = mMouse->getMouseState().X.rel;
            
        if (isSinglePlayer) {
            if (game->heliAI) {
                game->heliAI->move(-xMove, yMove, zMove);
                game->heliAI->rotate(-mMove*0.035);
            }
        }
        
        if (!isClient) {
            game->heli->move(xMove, yMove, zMove);
            game->heli->rotate(-mMove*0.035);
            game->heli->updateTransform();
	    }

        game->rotateHeliProps(evt.timeSinceLastFrame);
 
        // get a packet from the server, then set the ball's position
        if (isClient) {
            // get state of the game from the server
            ServerToClient servData;
            if (client->recMsg(servData)) {
                game->setDataFromServer(servData);
                if (servData.meta.shutdown) {
                    mShutDown = true;
                }
            }
                
            // send the user input and state for our helicopter to the server
            ClientToServer cdata;
            cdata.xMove = xMove;
            cdata.yMove = yMove;
            cdata.zMove = zMove;
            cdata.mMove = mMove;
            cdata.firingRocket = clientFiringRocket;
            if(!game->heli->alive)
                cdata.respawned = true;
            client->sendMsg(cdata);
            clientFiringRocket = false;
        } else {
            // step the simulator
            simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1/60.0f);

            if (!isSinglePlayer) {
                int newClientIndex = server->awaitConnections();
               
                if (newClientIndex != -1) {
                    assert(newClientIndex != 0 && "we can't add the server player as a new player!");
                    game->makeNewHeli(newClientIndex);
                }
 
                if (server->numConnected > 0) {    
                    // send the state of the game to the client
                    server->sendMsg(game->getServerToClientData());
                    simulator->soundPlayed = NOSOUND;
                
                    // get clients' user input and update state
                    for (int i = 0; i < NUM_PLAYERS - 1; i++) {
                        ClientToServer cdata;
                        if (server->recMsg(cdata, i)) {
                            cdata.respawned = false;
                            game->setDataFromClient(cdata, i+1);
                            if(game->helis[i+1] != NULL){
                                game->helis[i+1]->inBounds(game->level->getBounds(), evt.timeSinceLastFrame, NULL);
                                if(!game->helis[i+1]->alive){
                                    game->helis[i+1]->respawn(game->getSpawnPos(), evt.timeSinceLastFrame, NULL);
                                }
                            }
                            if (cdata.disconnecting) {
                                server->removeConnection(i);
                                printf("deleted connected to client #%d\n", i);
                            }
                        }
                    }
                }
            }
        }
	}
    return true;
}

bool Whirlybirds::keyPressed(const OIS::KeyEvent &arg)
{   
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	sys.injectChar(arg.text);

	if (simulator) {
		if (arg.key == OIS::KC_X) {
		    simulator->soundSystem->mute();
		} else if (arg.key == OIS::KC_C) {
			//simulator->soundSystem->playMusic();
		}
	}
	if (arg.key == OIS::KC_ESCAPE)
    {
        if (!isSinglePlayer) {
            if (isClient) {
                ClientToServer data;
                data.disconnecting = true;
                client->sendMsg(data);
            } else {            
                ServerToClient data;
                data.meta.shutdown = true;
                server->sendMsg(data);
            }
        }

        mShutDown = true;
    }
	return true;
}

bool Whirlybirds::keyReleased(const OIS::KeyEvent &arg)
{
	CEGUI::System::getSingleton().injectKeyUp(arg.key);
	return true;
}

CEGUI::MouseButton Whirlybirds::convertButton(OIS::MouseButtonID buttonID) {
	switch (buttonID) {
		case OIS::MB_Left:
			return CEGUI::LeftButton;
		case OIS::MB_Right:
			return CEGUI::RightButton;
		case OIS::MB_Middle:
			return CEGUI::MiddleButton;
		default:
			return CEGUI::LeftButton;
	}
}

bool Whirlybirds::mouseMoved(const OIS::MouseEvent &arg)
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	if (arg.state.Z.rel)
		sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}

bool Whirlybirds::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
 /*  if(id == OIS::MB_Left)
    {
        if (gameplay && (!isClient || isSinglePlayer)) {
            game->addRocket(game->heli);
            simulator->soundSystem->playShootRocket();
        } else {
            clientFiringRocket = true;
        }
    }*/
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	return true;
}

bool Whirlybirds::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;
}

void Whirlybirds::attachCamera(void) {
	(&(game->heli->getNode()))->createChildSceneNode("camNode");
	mSceneMgr->getSceneNode("camNode")->attachObject(mCamera);
	//mSceneMgr->getSceneNode("camNode")->translate(0.0, 35.0, 30.0);
    mSceneMgr->getSceneNode("camNode")->translate(0.0, 35.0, 60.0);
}

bool Whirlybirds::singlePlayer(const CEGUI::EventArgs &e)
{
    isClient = false;
    isSinglePlayer = true;
    clientFiringRocket = false;

	simulator->soundSystem->playMusic();
	simulator->soundSystem->playRotor();

    //simulator = new Simulator();
    game = new Game(simulator, mSceneMgr, isClient, isSinglePlayer);
    attachCamera();

	gui->destroyMenu(true);
    gameplay = true;
    return true;
}

bool Whirlybirds::clientStart(const CEGUI::EventArgs &e)
{
	isClient = true;
    isSinglePlayer = false;
	
	simulator->soundSystem->playMusic();
	simulator->soundSystem->playRotor();

    int sPort = gui->getPort();
	char* sip = gui->getIP();
    client = new Client(sip, sPort);

	if (client->serverFound) {
		//simulator = new Simulator();
        game = new Game(simulator, mSceneMgr, isClient, isSinglePlayer);

		gui->destroyMenu(false);
		gameplay = true;
        
        // get state of the game from the server
        ServerToClient servData;
        if (client->recMsg(servData)) {
            game->setDataFromServer(servData);
            attachCamera();
        }

        printf("@#$ client starting up...\n");
	} else {
        printf("ERROR: client couldn't find server!\n");
    }
    
	return true;
}

bool Whirlybirds::serverStart(const CEGUI::EventArgs &e)
{
	isClient = false;
    isSinglePlayer = false;

	simulator->soundSystem->playMusic();
	simulator->soundSystem->playRotor();
	
    int sPort = gui->getPort();
    server = new Server(sPort);
	
    //simulator = new Simulator();
    game = new Game(simulator, mSceneMgr, isClient, isSinglePlayer);
    attachCamera();
 
	gui->destroyMenu(false);
	gameplay = true;
    printf("@#$ Server starting up...\n");
	return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Whirlybirds app;
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
