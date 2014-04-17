/*
-----------------------------------------------------------------------------
Filename:    Whirlybirds.h
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
#ifndef __Whirlybirds_h_
#define __Whirlybirds_h_

#include "UDPNetEnt.h"
#include <btBulletDynamicsCommon.h>
#include "BaseApplication.h"
#include "Ball.h"
#include "Box.h"
#include "Simulator.h"
#include "Surface.h"
#include "Wall.h"
#include "Target.h"
#include "Client.h"
#include "Server.h"
#include "Heli.h"
#include "GUI.h"
#include "Level.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "ServerToClient.h"
#include "ClientToServer.h"
#include "Game.h"

class Whirlybirds : public BaseApplication
{

protected:
	GUI* gui;
    Ogre::ManualObject* manualInd;
    Simulator* simulator;
    Server* server;
    Client* client;
    bool isClient;

	// Game Objects
    Game* game;

    // OIS::KeyListener
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

    // OIS::MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
 
	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
    bool singlePlayer(const CEGUI::EventArgs &e);
	bool clientStart(const CEGUI::EventArgs &e);
	bool serverStart(const CEGUI::EventArgs &e);
    ServerToClient* initServerToClient();
    void updateIndicator(Ball* ball);
    void attachCamera(void);

    //debugging
    void printNodes(Ogre::SceneNode::ChildNodeIterator it, Ogre::String indent);
    
public:
    Whirlybirds();
    virtual ~Whirlybirds(void);
};

#endif // #ifndef __Whirlybirds_h_
