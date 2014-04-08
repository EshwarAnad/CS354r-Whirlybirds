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
{
    simulator = NULL;
}

//-------------------------------------------------------------------------------------
Whirlybirds::~Whirlybirds(void)
{
}

float indoffset = .5;

Ogre::Vector3 indPoints[7] = {
    Ogre::Vector3(0.0+indoffset, 0.05, 0.0),
    Ogre::Vector3(0.3+indoffset, 0.05, 0.0),
    Ogre::Vector3(0.3+indoffset, 0.1, 0.0),
    Ogre::Vector3(0.4+indoffset, 0.0, 0.0),
    Ogre::Vector3(0.3+indoffset, -0.1, 0.0),
    Ogre::Vector3(0.3+indoffset, -0.05, 0.0),
    Ogre::Vector3(0.0+indoffset, -0.05, 0.0)
};

int startingFace = 0;
bool gameplay = false;
bool isSinglePlayer = false;
int sPort = 49152;
char* sip;

//-------------------------------------------------------------------------------------
void Whirlybirds::createScene(void)
{
	CEGUI::Event::Subscriber* spSub = new CEGUI::Event::Subscriber(&Whirlybirds::singlePlayer, this);
	CEGUI::Event::Subscriber* clientSub = new CEGUI::Event::Subscriber(&Whirlybirds::clientStart, this);
	CEGUI::Event::Subscriber* serverSub = new CEGUI::Event::Subscriber(&Whirlybirds::serverStart, this);
	gui = new GUI(spSub, clientSub, serverSub);
	   
	// Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(10.0f, 10.0f, 10.0f);

	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
}

bool Whirlybirds::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    
    static Ogre::Real z_time = 0.0;
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
		xMove = 0.0,
		yMove = 0.0,
		zMove = 0.0;

        if(z_time > 0.0 && z_time < 1.0)
            z_time += evt.timeSinceLastFrame;
        else
            z_time = 0.0;
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

		p1Heli->move(xMove, yMove, zMove);
        
        Ogre::Real xMove = mMouse->getMouseState().X.rel;
        p1Heli->rotate(-xMove*0.05);
        p1Heli->updateTransform();
        // get a packet from the server, then set the ball's position
        if (isClient) {
            ServerToClient servData;
            
            // get data from the server
            if (client->recMsg(reinterpret_cast<char*>(&servData))) {
            }
    
        } else {
			p1Heli->animate(evt.timeSinceLastFrame);

            if(!isSinglePlayer){
                server->awaitConnections();
                // step the server's simulator
                simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1/60.0f);
                // send the state of the target to the client
                ServerToClient* data = initServerToClient();
                server->sendMsg(reinterpret_cast<char*>(data), sizeof(ServerToClient));
                simulator->soundPlayed = NOSOUND;
                delete data;
            
                // get the state of the p2HeliObj from the client
                ClientToServerData cdata;
                if (server->recMsg(reinterpret_cast<char*>(&cdata))) {
                }
            } else {
                simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1/60.0f);
            } 
        }

		if(!isClient){
		}
	}
    return true;
}

void Whirlybirds::updateIndicator(Ball* ball) {
    Ogre::Vector3 world_point = ball->getNode().getPosition();

    bool isBallVisible = mCamera->isVisible(Ogre::Sphere(world_point, 1.0));
    if (!isBallVisible) {
        Ogre::Vector3 screen_point = mCamera->getProjectionMatrix() * (mCamera->getViewMatrix() * world_point);  
        
        float angle = atan2(screen_point.x, screen_point.y);

        manualInd->beginUpdate(0);
        Ogre::Quaternion rot(Ogre::Radian(-angle + M_PI/2.0), Ogre::Vector3::UNIT_Z);
        for (int i = 0; i < 7; i++) {
            manualInd->position(rot * indPoints[i]);
        }
        manualInd->end();
    } else {
        manualInd->beginUpdate(0);
        for (int i = 0; i < 7; i++) {
            Ogre::Vector3 point = indPoints[i];
            point.z = -1000;
            manualInd->position(point);
        }
        manualInd->end();
    }
}

ServerToClient* Whirlybirds::initServerToClient(){
    ServerToClient* data = new ServerToClient();
    
    return data;
}

bool Whirlybirds::keyPressed(const OIS::KeyEvent &arg)
{   
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	sys.injectChar(arg.text);

	if (simulator) {
		if (arg.key == OIS::KC_X) {
			simulator->soundOn = !(simulator->soundOn);
		} else if (arg.key == OIS::KC_C) {
			simulator->soundSystem->playMusic();
		}
	}
	if (arg.key == OIS::KC_ESCAPE)
    {
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
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	return true;
}

bool Whirlybirds::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;
}

void Whirlybirds::printNodes(Ogre::SceneNode::ChildNodeIterator it, Ogre::String indent){
    Ogre::SceneNode* cur;
    while(it.hasMoreElements()){
        cur = dynamic_cast<Ogre::SceneNode*>(it.getNext());
        std::cout << indent << cur->getName() << std::endl;
        printNodes(cur->getChildIterator(), indent + "\t");
    }

}

void Whirlybirds::createSceneObjects() {
    static Ogre::Real WORLDSCALE = 3.0;
    Ogre::Vector3 origin(0, 0, 0);
    level = new Level("mylevel", mSceneMgr, simulator, WORLDSCALE, origin, 0.9, 0.1, "");
    Box* box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/BeachStones");
	p1Heli = new Heli("p1Heli", mSceneMgr, simulator, WORLDSCALE, 1.0, Ogre::Vector3(0.0, 0.0, 45.0), 0.9, 0.1, "Game/Helicopter");

    //iterate through all childs of root (debugging purposes)
    Ogre::SceneNode* theRoot = mSceneMgr->getRootSceneNode();
    Ogre::SceneNode::ChildNodeIterator rootIt = theRoot->getChildIterator();
    printNodes(rootIt, "");
	 if (!isSinglePlayer) {
     }

}

bool Whirlybirds::singlePlayer(const CEGUI::EventArgs &e)
{
    isClient = false;
    isSinglePlayer = true;

    simulator = new Simulator();

	createSceneObjects();

	(&(p1Heli->getNode()))->createChildSceneNode("camNode");
	mSceneMgr->getSceneNode("camNode")->attachObject(mCamera);
	mSceneMgr->getSceneNode("camNode")->translate(0.0, 30.0, 30.0);

	p1Heli->addToSimulator();
	p1Heli->setKinematic();
    level->addToSimulator();

	gui->destroyMenu(true);
    gameplay = true;
    return true;
}

bool Whirlybirds::clientStart(const CEGUI::EventArgs &e)
{
	isClient = true;
	sPort = gui->getPort();
	sip = gui->getIP();
    client = new Client(sip, sPort);

	if (client->serverFound) {
		simulator = new Simulator();

		gui->destroyMenu(false);
		gameplay = true;
	}
	return true;
}

bool Whirlybirds::serverStart(const CEGUI::EventArgs &e)
{
	isClient = false;
	sPort = gui->getPort();
    server = new Server(sPort);
	
    simulator = new Simulator();
  
    // Create a scene
    createSceneObjects();   
 
	gui->destroyMenu(false);
	gameplay = true;
    printf("Server starting up...\n");
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
